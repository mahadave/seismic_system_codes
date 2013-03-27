package communication;

/**
 *
 * @author apurvaa
 */

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.*;
import javax.comm.*;
import javax.swing.JTextArea;

public class SCI implements SerialPortEventListener {

    //private static Enumeration portList;
    private static JTextArea parent;
    private static CommPortIdentifier portId;
    private static SerialPort port;
    private static OutputStream outputStream;
    private static InputStream inputStream;
    //private MouseControlView UI;
    //private MazePanel maze;
    private byte[] readBuffer = new byte[500];
    private byte lastReceived = '.';
    private boolean count = false;
    private byte upper;
    private byte lower;
    private byte entries = 0;
    private int numbytes;
    private short max = Short.MIN_VALUE;
    private int filecount=0;
    int counterfileline=0;
    private short min = Short.MAX_VALUE;
    public static PrintWriter pw;
    
    static SimpleDateFormat formatter1 = new SimpleDateFormat("EEE MMM dd HH:mm:ss:SSS");
    static SimpleDateFormat formatter2 = new SimpleDateFormat("yyyy");
    static String zone=" SGT ";

    public SCI() {
        parent = null;
    }

    public void openPort(String name) throws NoSuchPortException {
        System.out.println(" opened " + portId.getName());
        String driverName = "com.sun.comm.Win32Driver";
        try {
            pw = new PrintWriter(new BufferedWriter(new FileWriter("data_DS2_"+(filecount++)+".txt")));
            pw.println("Time, Voltage");
            CommDriver commdriver = (CommDriver) Class.forName(driverName).newInstance();
            commdriver.initialize();
        } catch (Exception e2) {
            e2.printStackTrace();
        }

        portId = CommPortIdentifier.getPortIdentifier(name);
        System.out.println("port identified");
        if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
            if (portId.getName().equals(name)) {
                try {
                    port = (SerialPort) portId.open("portSetting", 20000);
                    System.out.println("Opened");
                } catch (PortInUseException e) {
                    System.out.println("failed");
                } catch (Exception e) {
                    System.out.println("e.getMessage");
                }
                try {
                    outputStream = port.getOutputStream();
                    inputStream = port.getInputStream();
                    port.addEventListener(this);//add listener
                    port.notifyOnDataAvailable(true);
                } catch (TooManyListenersException e) {
                    port.close();
                    System.out.println("listener failed");
                } catch (IOException e) {
                    System.out.println("error" + e.getMessage());
                    port.close();
                }
            }
        }
    }

    public void closePort() {
        port.close();
    }

    public void setPortParams(int baudrate, int dataBits, int stopBits, int parity) {
        try {
            port.setSerialPortParams(baudrate, dataBits, stopBits, parity);
            port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
        } catch (Exception e) {
            port.close();
        }
    }

    public void sendChar(char ch) {
        try {
            System.out.println("sending... " + ch);
            outputStream.write(ch);
            System.out.println("character sent");
        } catch (Exception e) {
            System.out.println(" Cannot send ");
        }
    }

    public void sendStr(byte[] c) {
        try {
            outputStream.write(c);
        } catch (Exception e) {
            // UI.textArea.append("\n"+e.getMessage());
        }
    }

    public byte getReceivedChar() {//use listener to get receving data
        return lastReceived;
    }

    public void serialEvent(SerialPortEvent event) throws IOException{//override event method

        switch (event.getEventType()) {
            case SerialPortEvent.BI:
            case SerialPortEvent.OE:
            case SerialPortEvent.FE:
            case SerialPortEvent.PE:
            case SerialPortEvent.CD:
            case SerialPortEvent.CTS:
            case SerialPortEvent.DSR:
            case SerialPortEvent.RI:
            case SerialPortEvent.OUTPUT_BUFFER_EMPTY:
                break;
            case SerialPortEvent.DATA_AVAILABLE:
                try {
                    while (inputStream.available() > 0) {
                        numbytes = inputStream.read(readBuffer);
                        //System.out.println(numbytes + " ");
                    }
                } catch (IOException e) {
                }
                break;
        }
         
         
        for (int i = 0; i<numbytes; i++) 
        {
            lastReceived = readBuffer[i];
            //System.out.println("here " + lastReceived);
           // System.out.printf(" last ----> = %x \n" ,(byte) (lastReceived)    );

            if (parent == null) {
                return;
            }

            if (entries == 11) {
                parent.setText("");
                entries = 0;
            }

            if (!count) {
                count = !count;
                upper = (byte) (lastReceived);
              //  System.out.println("upper " + upper);


            } else {
                count = !count;
                lower = (byte) (lastReceived);
            //    System.out.println("lower " + (lower));
                //short num = (short) ( (short)((0x00FF)&((lower))) + (short) ((0xFF00)&((upper)<<8)) );
                short num = (short) ((short)((upper&0x00FF) << 8) | (short)(0x00FF&lower));
                System.out.printf(".... %x ....  %x \n", upper, lower);
                System.out.printf(".... %x...... \n", num);
                System.out.println("----num = "+(num));
                min = (short) Math.min(min, num);
                max = (short) Math.max(max, num);
                String s="";
                Date tmp=new Date();
                String formattedDate = formatter1.format(tmp)+zone+formatter2.format(tmp);
                if(upper<0)
                    s = formattedDate + ", -" + Math.abs(num);
                else
                    s=  formattedDate + ", " + Math.abs(num);
                
                pw.println(s);
                
                
                
                //System.out.println("Today’s date and Time is:"+formattedDate);
                //pw.println(formattedDate);
                
                counterfileline++;
                if(counterfileline==50000)
                {
                    counterfileline=0;
                    pw.flush();
                    pw = new PrintWriter(new BufferedWriter(new FileWriter("data_DS2_"+(filecount++)+".txt")));
                    pw.println("Time, Voltage");
                    
                }
                parent.append(s + "\n");
         //       String intmin = Integer.toBinaryString(min);
       //         String intmax = Integer.toBinaryString(max);
     //           System.out.println(" last received = " + num + ", (min,max) = ("
           //             + min + "," + max + ") " + conv(intmin) + " " + conv(intmax));
                entries++;

                //upper=0;
                //lower=0;
            } // 2 bytes received
        }





    }

    protected void listPortChoices() {
        Enumeration en = CommPortIdentifier.getPortIdentifiers();

        // iterate through the ports.
        while (en.hasMoreElements()) {
            portId = (CommPortIdentifier) en.nextElement();

            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                System.out.println(portId.getName());
            }
        }
    }

    void setParent(JTextArea logger) {
        parent = logger;
    }

    private int conv(String intmin) {
        int mult = 1;
        int res = 0;
        for (int i = intmin.length() - 1; i >= 0; i--) {
            if (intmin.charAt(i) == '1') {
                res += mult;
            }
            mult *= 2;
        }
        return res;
    }
}
