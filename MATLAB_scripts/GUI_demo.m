function [] = GUI_demo()
% Demonstrate how to use toggle buttons to mimic tabbed panels.  
% Creates a GUI with three toggle buttons which act as tabs.  One of the 
% tabs is not selectable until the user plots a random quartic by 
% pressing the pushbutton at the bottom of the screen.  The middle tab 
% shows the result of fitting (with polyfit) a polynomial to the "unknown" 
% quartic.  
%
% Suggested exercise:  Add another tab which contains help info for how to
% use the GUI.
%
% 
% Author:  Akshat Dave
% Date:    27-Apr-2012

clear all
close all
clc


global f;
global cur;
cur=1;
f=0;
%%
SCR = get(0,'Screensize');  % Get screensize.
S.fh = figure('numbertitle','off',... 
              'units','pixels',...
              'position',[SCR(3)/2-SCR(3)/4 ,SCR(4)/2-SCR(4)/3, 800, 600],...
              'name','GUI_35',...
              'resize','off');
S.ax = axes('units','pixels',...
            'position',[100 100 600 450]);
S.pb1 = uicontrol('style','pushbutton',...
                  'units','pixels',...
                  'position',[80 20 300 40],...
                  'string','Run Plotter',...
                  'fontsize',12);
S.pb2 = uicontrol('style','pushbutton',...
                  'units','pixels',...
                  'position',[480 20 300 40],...
                  'string','Stop',...
                  'fontsize',12);              
              
W = {'style','edit','units','pixels','position'};  % Save some typing.               
% Set remaining properties.
set(S.pb1,'callback',{@pb_call,S})  % Set the callbacks.
set(S.pb2,'callback',{@pb_call2,S})  % Set the callbacks.


function [] = pb_call(varargin)
% Callback for pushbutton.

global f;
global cur;
f=0;
folder='hall11_data';
files = dir(strcat(folder, '/*.txt'));

for i=cur:length(files) % do for all files
    
    [allDate,allVel,realDate]=analysis_fn(i);
    
    plot(allDate,allVel,'--k'); 
    title(['Seismic Amplitudes Collected at ',realDate,' ( ',num2str(allDate(2,:)),'-',num2str(allDate(end,:)),' )'])
    xlabel ('Relative time since start ');
    ylabel ('velocity amplitude');
    hold on;

    if f>0
        f=0;
        disp('... breaking')
        break;
    end
    refresh
    pause(0.01);
    cur = cur+1;
end

function [] = pb_call2(varargin)
global f;
    f=1;
%--- NEED A BUTTON TO STOP THIS
