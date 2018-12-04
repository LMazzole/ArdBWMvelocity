%Ballwurfmaschine
%Auswertung-Histogrammplot
%Luca Mazzoleni
%HSR Hochschule Rapperswil
%02.12.2018

clc;
clear;
close all;
%% Read Data
DataTable= readtable('Data.csv','HeaderLines',1);
data=table2array(DataTable(:,[4:9]));
IntLB1=data(:,1);
IntLB2=data(:,2);
IntLB1121=data(:,3);
IntLB1222=data(:,4);
PulsLB1=data(:,5);
PulsLB2=data(:,6);
%% Plot Histogram

nbins=50;
xlimit=[185 210];
ylimit=[0 200];
linewidth=3;

figure(1)
hold on
subplot(6,1,1);
histogram(IntLB1,nbins)
% histfit(IntLB1,nbins)
title('Interrupt LB1');
line([188.5 188.5], [0 300],'LineWidth',linewidth);
xlim(xlimit)
ylim(ylimit)


subplot(6,1,2);
histogram(IntLB2,nbins)
% histfit(IntLB2,nbins)
title('Interrupt LB2');
line([188.5 188.5], [0 300],'LineWidth',linewidth);
xlim(xlimit)
ylim(ylimit)

subplot(6,1,3);
histogram(IntLB1121,nbins)
% histfit(IntLB1121,nbins)
title('Interrupt LB11-21');
line([188.5 188.5], [0 300],'LineWidth',linewidth);
xlim(xlimit)
ylim(ylimit)

subplot(6,1,4);
histogram(IntLB1222,nbins)
% histfit(IntLB1222,nbins)
title('Interrupt LB12-22');
line([188.5 188.5], [0 300],'LineWidth',linewidth);
xlim(xlimit)
ylim(ylimit)

subplot(6,1,5);
histogram(PulsLB1,nbins)
% histfit(PulsLB1,nbins)
title('Polling LB1');
line([188.5 188.5], [0 300],'LineWidth',linewidth);
xlim(xlimit)
ylim(ylimit)

subplot(6,1,6);
histogram(PulsLB2,nbins)
% histfit(PulsLB2,nbins)
title('Polling LB2');
line([188.5 188.5], [0 300],'LineWidth',linewidth);
xlim(xlimit)
ylim(ylimit)
xlabel('Geschwindigkeit [km/h]');
hold off
%% Plot Data
xaxislen=[1:length(IntLB1)];
xlimit=[1 length(IntLB1)];
ylimit=[185 210];
linewidth=1;
linewidth1=2;
figure(2)
hold on
subplot(6,1,1);
plot(xaxislen,IntLB1,'LineWidth',linewidth1);
title('Interrupt LB1');
ylabel('Geschwindigkeit [km/h]');
line(xaxislen,ones(1,length(IntLB1))*188.5,'LineWidth',linewidth,'LineStyle','--');
xlim(xlimit)
ylim(ylimit)

subplot(6,1,2);
plot(xaxislen,IntLB2,'LineWidth',linewidth1)
% histfit(IntLB2,nbins)
title('Interrupt LB2');
line(xaxislen,ones(1,length(IntLB1))*188.5,'LineWidth',linewidth,'LineStyle','--');
ylim(ylimit)
xlim(xlimit)
ylabel('Geschwindigkeit [km/h]');

subplot(6,1,3);
plot(xaxislen,IntLB1121,'LineWidth',linewidth1)
% histfit(IntLB1121,nbins)
title('Interrupt LB11-21');
line(xaxislen,ones(1,length(IntLB1))*188.5,'LineWidth',linewidth,'LineStyle','--');
ylim(ylimit)
xlim(xlimit)
ylabel('Geschwindigkeit [km/h]');

subplot(6,1,4);
plot(xaxislen,IntLB1222,'LineWidth',linewidth1)
% histfit(IntLB1222,nbins)
title('Interrupt LB12-22');
line(xaxislen,ones(1,length(IntLB1))*188.5,'LineWidth',linewidth,'LineStyle','--');
ylim(ylimit)
xlim(xlimit)
ylabel('Geschwindigkeit [km/h]');

subplot(6,1,5);
plot(xaxislen,PulsLB1,'LineWidth',linewidth1)
% histfit(PulsLB1,nbins)
title('Polling LB1');
line(xaxislen,ones(1,length(IntLB1))*188.5,'LineWidth',linewidth,'LineStyle','--');
ylim(ylimit)
xlim(xlimit)
ylabel('Geschwindigkeit [km/h]');

subplot(6,1,6);
plot(xaxislen,PulsLB2,'LineWidth',linewidth1)
% histfit(PulsLB2,nbins)
title('Polling LB2');
line(xaxislen,ones(1,length(IntLB1))*188.5,'LineWidth',linewidth,'LineStyle','--');
ylim(ylimit)
xlim(xlimit)
ylabel('Geschwindigkeit [km/h]');
xlabel('Sample');
hold off
%% Berechnung Verteilung
figure(99)
hold on
hist_IntLB1=histfit(IntLB1,nbins);
hist_IntLB1=hist_IntLB1(2);
hist_IntLB1X=hist_IntLB1.XData;
hist_IntLB1Y=hist_IntLB1.YData;

hist_IntLB2=histfit(IntLB2,nbins);
hist_IntLB2=hist_IntLB2(2);
hist_IntLB2X=hist_IntLB2.XData;
hist_IntLB2Y=hist_IntLB2.YData;

hist_IntLB1121=histfit(IntLB1121,nbins);
hist_IntLB1121=hist_IntLB1121(2);
hist_IntLB1121X=hist_IntLB1121.XData;
hist_IntLB1121Y=hist_IntLB1121.YData;

hist_IntLB1222=histfit(IntLB1222,nbins);
hist_IntLB1222=hist_IntLB1222(2);
hist_IntLB1222X=hist_IntLB1222.XData;
hist_IntLB1222Y=hist_IntLB1222.YData;

hist_PulsLB1=histfit(PulsLB1,nbins);
hist_PulsLB1=hist_PulsLB1(2);
hist_PulsLB1X=hist_PulsLB1.XData;
hist_PulsLB1Y=hist_PulsLB1.YData;

hist_PulsLB2=histfit(PulsLB2,nbins);
hist_PulsLB2=hist_PulsLB2(2);
hist_PulsLB2X=hist_PulsLB2.XData;
hist_PulsLB2Y=hist_PulsLB2.YData;
hold off
close(99)

figure(3)
hold on
plot(hist_IntLB1X,hist_IntLB1Y,'LineWidth',2)
plot(hist_IntLB2X,hist_IntLB2Y,'LineWidth',2)
plot(hist_PulsLB1X,hist_PulsLB1Y,'--','LineWidth',2)
plot(hist_PulsLB2X,hist_PulsLB2Y,'--','LineWidth',2)
plot(hist_IntLB1121X,hist_IntLB1121Y,'LineWidth',2)
plot(hist_IntLB1222X,hist_IntLB1222Y,'LineWidth',2)
% title('Histogram');
legend('Interrupt LB1', ...
        'Interrupt LB2',...
        'Polling LB1',...
        'Polling LB2',...
        'Interrupt LB11-21',...
        'Interrupt LB12-22');
xlabel('Geschwindigkeit [km/h]');
line([188.5 188.5], [0 300],'LineWidth',2);
xlim([185 205])
ylim([0 30])
hold off

%% Ausgabe
mean_data=mean(data,'omitnan');
std_data=std(data,'omitnan');
speed = 188.5; %Sollwert

genau_data=abs(speed-mean_data);
genau_data_proz=genau_data/speed*100;
prez_data_proz=std_data./mean_data*100;

disp('===Auswertung===')
disp('Genauigkeit [km/h]')
disp('   IntLB1     IntLB2  IntLB1121  IntLB1222   PulsLB1   PulsLB2')
disp(genau_data)
disp('Prazision [km/h]')
disp('   IntLB1     IntLB2  IntLB1121  IntLB1222   PulsLB1   PulsLB2')
disp(std_data)
disp('Genauigkeit [%]')
disp('   IntLB1     IntLB2  IntLB1121  IntLB1222   PulsLB1   PulsLB2')
disp(genau_data_proz)
disp('Prazision [%]')
disp('   IntLB1     IntLB2  IntLB1121  IntLB1222   PulsLB1   PulsLB2')
disp(prez_data_proz)

