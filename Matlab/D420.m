%% 数据输入与预处理
data = [
140	25	490.225
140	26	490.225
140	27	490.225
140	28	491
140	29	491
140	30	491
140	31	491
140	32	491
140	33	491
140	34	491
140	35	491
140	36	491
140	37	491
140	38	491
140	39	491
140	40	491
140	41	491
140	42	491
140	43	491
140	44	491
140	45	491
140	46	491
140	47	491
140	48	491.775
140	49	491.775
140	50	491.775
140	51	491.775
140	52	491.775
140	53	491.775
140	54	491.775
140	55	491.775
140	56	491.775
140	57	491.775
140	58	491.775
140	59	491.775
140	60	491.775
140	61	491.775
140	62	491.775
140	63	491.775
140	64	491.775
140	65	491.775
280	25	489.451
280	26	489.451
280	27	489.451
280	28	489.451
280	29	489.451
280	30	489.451
280	31	490.225
280	32	490.225
280	33	490.225
280	34	490.225
280	35	490.225
280	36	490.225
280	37	490.225
280	38	490.225
280	39	490.225
280	40	490.225
280	41	490.225
280	42	490.225
280	43	490.225
280	44	490.225
280	45	490.225
280	46	490.225
280	47	490.225
280	48	490.225
280	49	490.225
280	50	490.225
280	51	490.225
280	52	491
280	53	491
280	54	491
280	55	491
280	56	491
280	57	491
280	58	491
280	59	491
280	60	491
280	61	491
280	62	491
280	63	491
280	64	491
280	65	491
420	25	488.676
420	26	488.676
420	27	489.451
420	28	489.451
420	29	489.451
420	30	489.451
420	31	489.451
420	32	489.451
420	33	489.451
420	34	489.451
420	35	489.451
420	36	489.451
420	37	489.451
420	38	489.451
420	39	489.451
420	40	489.451
420	41	489.451
420	42	489.451
420	43	489.451
420	44	489.451
420	45	489.451
420	46	490.225
420	47	490.225
420	48	490.225
420	49	490.225
420	50	490.225
420	51	490.225
420	52	490.225
420	53	490.225
420	54	490.225
420	55	490.225
420	56	490.225
420	57	490.225
420	58	490.225
420	59	490.225
420	60	490.225
420	61	490.225
420	62	490.225
420	63	490.225
420	64	490.225
420	65	490.225
560	25	487.901
560	26	487.901
560	27	488.676
560	28	488.676
560	29	488.676
560	30	488.676
560	31	488.676
560	32	488.676
560	33	488.676
560	34	488.676
560	35	488.676
560	36	488.676
560	37	488.676
560	38	488.676
560	39	489.451
560	40	489.451
560	41	489.451
560	42	489.451
560	43	489.451
560	44	489.451
560	45	489.451
560	46	489.451
560	47	489.451
560	48	489.451
560	49	489.451
560	50	489.451
560	51	489.451
560	52	489.451
560	53	489.451
560	54	489.451
560	55	489.451
560	56	489.451
560	57	490.225
560	58	490.225
560	59	490.225
560	60	490.225
560	61	490.225
560	62	490.225
560	63	490.225
560	64	490.225
560	65	490.225
700	25	487.901
700	26	487.901
700	27	487.901
700	28	487.901
700	29	487.901
700	30	488.676
700	31	488.676
700	32	488.676
700	33	488.676
700	34	488.676
700	35	488.676
700	36	488.676
700	37	488.676
700	38	488.676
700	39	488.676
700	40	488.676
700	41	488.676
700	42	488.676
700	43	488.676
700	44	488.676
700	45	488.676
700	46	488.676
700	47	488.676
700	48	488.676
700	49	489.451
700	50	489.451
700	51	489.451
700	52	489.451
700	53	489.451
700	54	489.451
700	55	489.451
700	56	489.451
700	57	489.451
700	58	489.451
700	59	489.451
700	60	489.451
700	61	489.451
700	62	489.451
700	63	490.225
700	64	490.225
700	65	490.225

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
I_pred = 420; T_pred = 25;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end
% 示例：预测电流=300mA, 温度=40℃时的参数
I_pred = 420; T_pred = 30;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 420; T_pred = 35;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 420; T_pred = 40;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 420; T_pred = 45;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 420; T_pred = 50;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 420; T_pred = 55;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 420; T_pred = 60;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 420; T_pred = 65;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end