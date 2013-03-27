% improve x-axis

clear all
close all
clc

allVel=[];
allDate=[];
t=[];
%prefix='data_DSLab_';
prefix='data_DS2_';%'data_Hall_11_';%
%folder='DSLab_data';
folder='C:\dc\d2\NetBeansProjects\SeismometerVirtualDataLogger\';%'DS2';%'DSLab_data'; 
offset=10;
files = dir(strcat(folder, '/*.txt'));

initTime=[];
finalTime=[];

figure;
clf;


for i=1:length(files) % do for all files
%%

    
    
    
    allVel=[];
    allDate=[];
    disp('waiting...')
    pause(0.001); % pause for file to be written first
    disp('processing .')
    %filename = files(i).name;
    filename = [prefix,num2str(i-1),'.txt'];
    file = fullfile(folder,filename);    
    
    fid = fopen(file);
    while fid<0
        fid = fopen(file);
        pause(5); % wait here for file to be written
    end
    disp(['processing:',filename]);

    B=textscan(fid,'%s %s', 'delimiter',',');
    date = B{1};
    adc = B{2};

    
    
    fclose(fid);

%%

    dateVal = date(2);
    dateVal = dateVal{1};
    realDate = [substring(dateVal,0,10) , ',', ...
    substring(dateVal,size(dateVal,2)-8,size(dateVal,2)-1)];

%%

    date=str2mat(date);
    date = date(:,11:23);
    %%
    date = date(2:end,:);
    date2 = ['',date(:,2:3),date(:,5:6),date(:,8:9),date(:,11:13),''];
    finalDate = str2num([date2(:,:)]);
    
    if(isempty(initTime))
        initTime = date(1,:);
    end
    
    
    finalTime= date(end,:);
    
    %%

    ADC = str2double(adc);
    ADC = ADC(2:end) + offset; % voltage error in adc
%%

% convert adc readout to 


    amp = ADC*6.144/32768;
    velocity = amp/2; % in/sec
    velocity = velocity*0.0254; % m/s

    allVel = [allVel ; velocity];
    allDate = [allDate; finalDate];


%%


    
%{
    tap=5;
    t=50000*i+[1:(numel(allVel))];
    
    
    for k=1:(50000-tap)
        for j=k:k+tap
            s= allVel(j);
        end
        allVel(k+tap) = (s);
    end
    allVel(1:tap)=0;
%}
    t= 50000*i+[1:numel(allVel)];

    %plot(t,ADC,'-b'); 
    plot(t,-allVel,'-b'); 
    %plot(allDate,allVel,'--k'); 
    title(['Seismic Amplitudes Collected at ',realDate,' ( ',num2str(initTime),'-',num2str(finalTime),' )'])
    xlabel ('Relative time since start ');
    ylabel ('velocity amplitude');
    hold on;

    refresh
end