% 中间的东西
% salt_mass = [65.49,70.71,75.70,80.65,85.68,90.72,94.72,101.76,108.68,114.23,119.04,122.74,129.76,134.71];
% angle     = [10.54,11.70,12.82,13.94,15.16,18.87,19.79,19.14 ,24.38, 22.6,  25.3,  28.25, 28.77, 30.31];


salt_mass = [102.40 106.26 108.86 110.52 112.40 114.26  116.76   118.69 120.37];
angle =     [18.95  19.23  20.23  20.66  21.50   21.75   23.09   23.89  23.94 ] ;

% 拟合多项式
degree = 3; % 选择3阶多项式进行拟合
p = polyfit(angle, salt_mass, degree);

% 生成拟合曲线数据
angle_fit = linspace(min(angle), max(angle), 100);
salt_mass_fit = polyval(p, angle_fit);

% 绘图
figure;
plot(angle, salt_mass, 'o', 'DisplayName', 'Original Data'); % 原始数据点
hold on;
plot(angle_fit, salt_mass_fit, '-', 'DisplayName', ['Polynomial Fit (Degree ' num2str(degree) ')']); % 拟合曲线
xlabel('Angle');
ylabel('Salt Mass');
title('Polynomial Fit');
legend;
hold off;

% 输出多项式表达式
disp('Fitted polynomial coefficients (from highest to lowest degree):');
disp(p);
