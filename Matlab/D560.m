%% 数据输入与预处理
data = [
140	26	16.44738
140	31	16.29762
140	36	16.093
140	41	15.85738
140	46	15.60062
140	51	15.33075
140	56	15.09775
140	61	14.847
140	66	14.58575
140	71	14.28338
280	26	28.71562
280	31	28.22675
280	36	27.73
280	41	27.29662
280	46	26.787
280	51	26.26675
280	56	25.7905
280	61	25.34388
280	66	24.95112
280	71	24.41025
420	26	40.51062
420	31	39.758
420	36	39.066
420	41	38.34625
420	46	37.72625
420	51	37.00612
420	56	36.305
420	61	35.62
420	66	35.02988
420	71	34.34162
560	26	49.7255
560	31	48.64012
560	36	47.71412
560	41	46.90088
560	46	46.086
560	51	45.10888
560	56	44.28188
560	61	43.41225
560	66	42.58275
560	71	41.805
700	26	57.51712
700	31	56.29262
700	36	55.20062
700	41	54.19738
700	46	53.10788
700	51	52.18762
700	56	51.16988
700	61	50.39875
700	66	49.25638
700	71	48.33875


];

I = data(:,1);      % 电流(mA)
T = data(:,2);      % 温度(℃)
params = data(:,3); % [a, b, c, A1, A2, A3, x2]

%% 模型构建系统
% 二次多项式模型：Y = β0 + β1*I + β2*T + β3*I² + β4*T² + β5*I*T
X = [ones(size(I)), I, T, I.^2, T.^2, I.*T];

param_names = {'x1'};
units = {''}; % 参数单位

coefficients = cell(1,1);
predicted = zeros(size(params));
R2 = zeros(1,1);
RMSE = zeros(1,1);

%% 批量建模与评估
for i = 1:1
    % 最小二乘拟合
    coefficients{i} = X \ params(:,i);
    predicted(:,i) = X * coefficients{i};
    
    % 计算评估指标
    SSE = sum((params(:,i) - predicted(:,i)).^2);
    SST = sum((params(:,i) - mean(params(:,i))).^2);
    R2(i) = 1 - SSE/SST;
    RMSE(i) = sqrt(mean((params(:,i) - predicted(:,i)).^2));
    
    % 输出模型方程
    fprintf('\n参数 %s 的拟合方程:\n', param_names{i});
    fprintf('Y = %.5f + %.3e*I + %.5f*T + %.3e*I² + %.3e*T² + %.3e*I*T\n',...
            coefficients{i}(1), coefficients{i}(2), coefficients{i}(3),...
            coefficients{i}(4), coefficients{i}(5), coefficients{i}(6));
    fprintf('R² = %.4f, RMSE = %.4f %s\n', R2(i), RMSE(i), units{i});
end



%% 预测模块
% 示例：预测电流=300mA, 温度=40℃时的参数
I_pred = 560; T_pred = 25;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end
% 示例：预测电流=300mA, 温度=40℃时的参数
I_pred = 560; T_pred = 30;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 560; T_pred = 35;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 560; T_pred = 40;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 560; T_pred = 45;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 560; T_pred = 50;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 560; T_pred = 55;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 560; T_pred = 60;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 560; T_pred = 65;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end