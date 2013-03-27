/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package communication;

import UI.mainFrame;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;


/**
 *
 * @author dave
 */
public class MainComm 
{
    // config params
    
    public static void main(String g[])
    {
        mainFrame frame =  new mainFrame();
        
        
        String portName = "COM"+"1";//"COM20";//virtual
        int baud = 57600;
        int dataBits = 8;
        int stopBits = 1;
        int parity = 0;
        boolean opened = false;
    
        System.out.println("starting");
        SCI sci=new SCI();
        
        sci.listPortChoices();
        try{sci.openPort(portName);} 
        catch(Exception e){System.out.println("error");}
        
        sci.setPortParams(baud, dataBits, stopBits, parity);
        sci.setParent(frame.getLogger());
    }
}
