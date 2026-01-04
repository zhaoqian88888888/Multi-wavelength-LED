%% 数据输入与预处理
data = [
	
145	26	532.728
145	27	532.728
145	28	532.728
145	29	532.728
145	30	533.498
145	31	533.498
145	32	533.498
145	33	533.498
145	34	533.498
145	35	533.498
145	36	533.498
145	37	533.498
145	38	533.498
145	39	533.498
145	40	533.498
145	41	533.498
145	42	533.498
145	43	534.269
145	44	534.269
145	45	534.269
145	46	534.269
145	47	534.269
145	48	534.269
145	49	534.269
145	50	534.269
145	51	534.269
145	52	534.269
145	53	535.039
145	54	535.039
145	55	535.039
145	56	535.039
145	57	535.039
145	58	535.039
145	59	535.039
145	60	535.039
145	61	535.81
145	62	535.81
145	63	535.81
145	64	535.81
145	65	535.81
145	66	535.81
285	26	530.415
285	27	530.415
285	28	530.415
285	29	531.186
285	30	531.186
285	31	531.186
285	32	531.186
285	33	531.186
285	34	531.186
285	35	531.186
285	36	531.186
285	37	531.957
285	38	531.957
285	39	531.957
285	40	531.957
285	41	531.957
285	42	531.957
285	43	531.957
285	44	531.957
285	45	531.957
285	46	531.957
285	47	531.957
285	48	531.957
285	49	532.728
285	50	532.728
285	51	532.728
285	52	532.728
285	53	532.728
285	54	532.728
285	55	532.728
285	56	532.728
285	57	532.728
285	58	532.728
285	59	532.728
285	60	532.728
285	61	532.728
285	62	532.728
285	63	532.728
285	64	533.498
285	65	533.498
285	66	533.498
425.5	26	528.102
425.5	27	528.102
425.5	28	528.102
425.5	29	528.873
425.5	30	528.873
425.5	31	528.873
425.5	32	528.873
425.5	33	528.873
425.5	34	528.873
425.5	35	528.873
425.5	36	528.873
425.5	37	528.873
425.5	38	528.873
425.5	39	528.873
425.5	40	528.873
425.5	41	528.873
425.5	42	528.873
425.5	43	529.644
425.5	44	529.644
425.5	45	529.644
425.5	46	529.644
425.5	47	529.644
425.5	48	529.644
425.5	49	530.415
425.5	50	530.415
425.5	51	530.415
425.5	52	530.415
425.5	53	530.415
425.5	54	530.415
425.5	55	530.415
425.5	56	530.415
425.5	57	531.186
425.5	58	531.186
425.5	59	531.186
425.5	60	531.186
425.5	61	531.186
425.5	62	531.186
425.5	63	531.957
425.5	64	531.957
425.5	65	531.957
425.5	66	531.957
564.5	26	527.331
564.5	27	527.331
564.5	28	527.331
564.5	29	527.331
564.5	30	527.331
564.5	31	527.331
564.5	32	527.331
564.5	33	527.331
564.5	34	527.331
564.5	35	527.331
564.5	36	528.102
564.5	37	528.102
564.5	38	528.102
564.5	39	528.102
564.5	40	528.102
564.5	41	528.102
564.5	42	528.102
564.5	43	528.102
564.5	44	528.873
564.5	45	528.873
564.5	46	528.873
564.5	47	528.873
564.5	48	528.873
564.5	49	528.873
564.5	50	529.644
564.5	51	529.644
564.5	52	529.644
564.5	53	529.644
564.5	54	529.644
564.5	55	529.644
564.5	56	529.644
564.5	57	529.644
564.5	58	529.644
564.5	59	529.644
564.5	60	529.644
564.5	61	529.644
564.5	62	530.415
564.5	63	530.415
564.5	64	530.415
564.5	65	530.415
564.5	66	530.415
704.5	26	526.56
704.5	27	526.56
704.5	28	526.56
704.5	29	526.56
704.5	30	526.56
704.5	31	526.56
704.5	32	526.56
704.5	33	527.331
704.5	34	527.331
704.5	35	527.331
704.5	36	527.331
704.5	37	527.331
704.5	38	527.331
704.5	39	527.331
704.5	40	527.331
704.5	41	527.331
704.5	42	527.331
704.5	43	527.331
704.5	44	527.331
704.5	45	527.331
704.5	46	528.102
704.5	47	528.102
704.5	48	528.102
704.5	49	528.102
704.5	50	528.102
704.5	51	528.102
704.5	52	528.102
704.5	53	528.102
704.5	54	528.102
704.5	55	528.873
704.5	56	528.873
704.5	57	528.873
704.5	58	528.873
704.5	59	528.873
704.5	60	528.873
704.5	61	528.873
704.5	62	528.873
704.5	63	528.873
704.5	64	528.873
704.5	65	528.873
704.5	66	528.873



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
    fprintf('Y = %.5f + %.3e*A + %.5f*B + %.3e*A*A + %.3e*B*B + %.3e*A*B\n',...
            coefficients{i}(1), coefficients{i}(2), coefficients{i}(3),...
            coefficients{i}(4), coefficients{i}(5), coefficients{i}(6));
    fprintf('R² = %.4f, RMSE = %.4f %s\n', R2(i), RMSE(i), units{i});
end



%% 预测模块
% 示例：预测电流=300mA, 温度=40℃时的参数
I_pred = 140; T_pred = 25;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end
% 示例：预测电流=300mA, 温度=40℃时的参数
I_pred = 140; T_pred = 30;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 140; T_pred = 35;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 140; T_pred = 40;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 140; T_pred = 45;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 140; T_pred = 50;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 140; T_pred = 55;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 140; T_pred = 60;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end

I_pred = 140; T_pred = 65;
pred_X = [1, I_pred, T_pred, I_pred^2, T_pred^2, I_pred*T_pred];
pred_values = zeros(1,1);

fprintf('\n预测结果 @I=%dmA, T=%d℃:\n', I_pred, T_pred);
for i = 1:1
    pred_values(i) = pred_X * coefficients{i};
    fprintf('%s = %.5f %s\n', param_names{i}, pred_values(i), units{i});
end