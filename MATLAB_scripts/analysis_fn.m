function [allDate,allVel,realDate]=analysis_fn(i)

    folder='hall11_data';
    prefix='data_Hall_11_';
    allVel=[];
    allDate=[];
    disp('waiting...')
    
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
    date = date(:,11:19);
    date = date(2:end,:);
    date2 = ['',date(:,2:3),date(:,5:6),date(:,8:9),''];
    finalDate = str2num([date2(:,:)]);
    %%

    ADC = str2double(adc);
    ADC = ADC(2:end);
%%

% convert adc readout to 


    amp = ADC*6.144/32768;
    velocity = amp/2; % in/sec
    velocity = velocity*0.0254; % m/s

    allVel = [allVel ; velocity];
    allDate = [allDate; finalDate];


%%
end