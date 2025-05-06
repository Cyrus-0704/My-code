% === 1. 读取训练集 ===
train = readtable('model_train.xlsx');
X_train = [train.X1, train.X2, train.X3];
Y_train = train.Y;

% === 2. 拟合泊松回归模型 ===
mdl = fitglm(X_train, Y_train, 'Distribution', 'poisson', 'Link', 'log');

% === 3. 读取用于预测的数据 ===
test = readtable('model_input.xlsx');
X_test = [test.X1, test.X2, test.X3];

% === 4. 预测 7月21日新增关注数 ===
Y_pred = round(predict(mdl, X_test));
test.Y_pred_7_21 = Y_pred;

% === 5. 输出前 5 名预测最多博主 ===
sorted = sortrows(test, 'Y_pred_7_21', 'descend');
disp('Top 5 bloggers by predicted 7.21 follows:');
disp(sorted(1:5, {'ID', 'Y_pred_7_21'}));

% === 6. 保存预测结果 ===
writetable(sorted, '预测结果_7月21.xlsx');
