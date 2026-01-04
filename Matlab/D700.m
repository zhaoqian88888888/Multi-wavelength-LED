%% 数据输入与预处理
data = [
0.14	26	490.225
0.14	27	490.225
0.14	28	490.225
0.14	29	490.225
0.14	30	490.225
0.14	31	490.225
0.14	32	490.225
0.14	33	490.225
0.14	34	490.225
0.14	35	491
0.14	36	491
0.14	37	491
0.14	38	491
0.14	39	491
0.14	40	491
0.14	41	491
0.14	42	491
0.14	43	491
0.14	44	491
0.14	45	491
0.14	46	491
0.14	47	491
0.14	48	491
0.14	49	491
0.14	50	491.775
0.14	51	491.775
0.14	52	491.775
0.14	53	491.775
0.14	54	491.775
0.14	55	491.775
0.14	56	491.775
0.14	57	491.775
0.14	58	491.775
0.14	59	491.775
0.14	60	491.775
0.14	61	491.775
0.14	62	491.775
0.14	63	491.775
0.14	64	491.775
0.14	65	491.775
0.14	66	491.775
0.14	67	491.775
0.14	68	491.775
0.14	69	491.775
0.14	70	491.775
0.14	71	491.775
0.14	72	491.775
0.28	26	489.451
0.28	27	489.451
0.28	28	489.451
0.28	29	489.451
0.28	30	489.451
0.28	31	489.451
0.28	32	489.451
0.28	33	489.451
0.28	34	489.451
0.28	35	489.451
0.28	36	489.451
0.28	37	489.451
0.28	38	490.225
0.28	39	490.225
0.28	40	490.225
0.28	41	490.225
0.28	42	490.225
0.28	43	490.225
0.28	44	490.225
0.28	45	490.225
0.28	46	490.225
0.28	47	490.225
0.28	48	490.225
0.28	49	490.225
0.28	50	490.225
0.28	51	491
0.28	52	491
0.28	53	491
0.28	54	491
0.28	55	491
0.28	56	491
0.28	57	491
0.28	58	491
0.28	59	491
0.28	60	491
0.28	61	491
0.28	62	491
0.28	63	491
0.28	64	491
0.28	65	491
0.28	66	491.775
0.28	67	491.775
0.28	68	491.775
0.28	69	491.775
0.28	70	491.775
0.28	71	491.775
0.28	72	491.775
0.42	26	489.451
0.42	27	489.451
0.42	28	489.451
0.42	29	489.451
0.42	30	489.451
0.42	31	489.451
0.42	32	489.451
0.42	33	489.451
0.42	34	489.451
0.42	35	489.451
0.42	36	489.451
0.42	37	489.451
0.42	38	489.451
0.42	39	489.451
0.42	40	489.451
0.42	41	489.451
0.42	42	489.451
0.42	43	490.225
0.42	44	490.225
0.42	45	490.225
0.42	46	490.225
0.42	47	490.225
0.42	48	490.225
0.42	49	490.225
0.42	50	490.225
0.42	51	490.225
0.42	52	490.225
0.42	53	490.225
0.42	54	490.225
0.42	55	490.225
0.42	56	490.225
0.42	57	490.225
0.42	58	490.225
0.42	59	490.225
0.42	60	490.225
0.42	61	490.225
0.42	62	491
0.42	63	491
0.42	64	491
0.42	65	491
0.42	66	491
0.42	67	491
0.42	68	491
0.42	69	491
0.42	70	491
0.42	71	491
0.42	72	491
0.56	26	488.676
0.56	27	488.676
0.56	28	488.676
0.56	29	488.676
0.56	30	488.676
0.56	31	488.676
0.56	32	488.676
0.56	33	488.676
0.56	34	488.676
0.56	35	488.676
0.56	36	488.676
0.56	37	488.676
0.56	38	489.451
0.56	39	489.451
0.56	40	489.451
0.56	41	489.451
0.56	42	489.451
0.56	43	489.451
0.56	44	489.451
0.56	45	489.451
0.56	46	489.451
0.56	47	489.451
0.56	48	489.451
0.56	49	489.451
0.56	50	489.451
0.56	51	489.451
0.56	52	489.451
0.56	53	489.451
0.56	54	489.451
0.56	55	489.451
0.56	56	489.451
0.56	57	489.451
0.56	58	490.225
0.56	59	490.225
0.56	60	490.225
0.56	61	490.225
0.56	62	490.225
0.56	63	490.225
0.56	64	490.225
0.56	65	490.225
0.56	66	490.225
0.56	67	490.225
0.56	68	490.225
0.56	69	491
0.56	70	491
0.56	71	491
0.56	72	491
0.70 	26	487.901
0.70 	27	487.901
0.70 	28	487.901
0.70 	29	488.676
0.70 	30	488.676
0.70 	31	488.676
0.70 	32	488.676
0.70 	33	488.676
0.70 	34	488.676
0.70 	35	488.676
0.70 	36	488.676
0.70 	37	488.676
0.70 	38	488.676
0.70 	39	488.676
0.70 	40	488.676
0.70 	41	488.676
0.70 	42	488.676
0.70 	43	489.451
0.70 	44	489.451
0.70 	45	489.451
0.70 	46	489.451
0.70 	47	489.451
0.70 	48	489.451
0.70 	49	489.451
0.70 	50	489.451
0.70 	51	489.451
0.70 	52	489.451
0.70 	53	489.451
0.70 	54	489.451
0.70 	55	489.451
0.70 	56	489.451
0.70 	57	489.451
0.70 	58	489.451
0.70 	59	489.451
0.70 	60	489.451
0.70 	61	489.451
0.70 	62	490.225
0.70 	63	490.225
0.70 	64	490.225
0.70 	65	490.225
0.70 	66	490.225
0.70 	67	490.225
0.70 	68	490.225
0.70 	69	490.225
0.70 	70	490.225
0.70 	71	490.225
0.70 	72	490.225



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
I_pred = 700; T_pred = 25;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end
% 示例：预测电流=300mA, 温度=40℃时的参数
I_pred = 700; T_pred = 30;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 700; T_pred = 35;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 700; T_pred = 40;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 700; T_pred = 45;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 700; T_pred = 50;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 700; T_pred = 55;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 700; T_pred = 60;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 700; T_pred = 65;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end