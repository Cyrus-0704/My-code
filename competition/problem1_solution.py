import pandas as pd

import numpy as np

from sklearn.ensemble import RandomForestRegressor

from sklearn.model_selection import TimeSeriesSplit, GridSearchCV

from sklearn.metrics import mean_absolute_error, mean_squared_error

import warnings

warnings.filterwarnings('ignore')

# 1. 数据读取

# 假设附件1已保存为 'attachment1.csv'，包含列: UserID, Behavior, BloggerID, Time

df = pd.read_csv(r"E:\Code\code\competition\attachment1.csv", parse_dates=['Time'])



# 2. 数据预处理

# 筛选关注行为(Behavior==4)

df_follow = df[df['Behavior'] == 4].copy()

# 提取日期

df_follow['Date'] = df_follow['Time'].dt.date

# 统计每个博主每日关注数

daily_follow = (

df_follow

.groupby(['BloggerID', 'Date'])

.size()

.reset_index(name='follow_count')

)

# 生成完整时间序列 (2024-07-11 至 2024-07-20)

blogger_ids = daily_follow['BloggerID'].unique()

dates = pd.date_range('2024-07-11', '2024-07-20').date

tpl = pd.MultiIndex.from_product([blogger_ids, dates], names=['BloggerID','Date'])

full = pd.DataFrame(index=tpl).reset_index()

daily = full.merge(daily_follow, on=['BloggerID','Date'], how='left').fillna(0)

# 3. 特征工程

daily = daily.sort_values(['BloggerID','Date'])

# 滑动窗口特征
def rolling_feat(group):

    group = group.copy()

    group['roll3_mean'] = group['follow_count'].rolling(3, min_periods=1).mean()

    group['roll5_mean'] = group['follow_count'].rolling(5, min_periods=1).mean()

    group['diff1'] = group['follow_count'].diff(1).fillna(0)

    group['rate1'] = group['diff1'] / (group['follow_count'].shift(1).replace(0, np.nan))

    group['rate1'] = group['rate1'].fillna(0)

    return group

daily = daily.groupby('BloggerID').apply(rolling_feat).reset_index(drop=True)

# 可以加入当日其他交互特征（观看、点赞、评论）

# 假设已提前统计好 daily_views, daily_likes, daily_comments 类似 daily_follow

# 这里以0填充示例

daily['views'] = 0

daily['likes'] = 0

daily['comments'] = 0

# 交互率

daily['like_rate'] = daily['likes'] / daily['views'].replace(0, np.nan)

daily['comment_rate'] = daily['comments'] / daily['views'].replace(0, np.nan)

daily[['like_rate','comment_rate']] = daily[['like_rate','comment_rate']].fillna(0)

# 4. 构建训练、验证集

daily['day_index'] = (pd.to_datetime(daily['Date']) - pd.to_datetime('2024-07-11')).dt.days + 1

daily = daily.sort_values(['BloggerID','day_index'])

# 训练: day_index 1-9, 验证: day_index 10

d_train = daily[daily['day_index'] <= 9]

d_val = daily[daily['day_index'] == 10]

features = ['roll3_mean', 'roll5_mean', 'diff1', 'rate1', 'views', 'likes', 'comments', 'like_rate', 'comment_rate']

target = 'follow_count'

# 5. 模型训练 (随机森林示例)

X_train = d_train[features]

y_train = d_train[target]

X_val = d_val[features]

y_val = d_val[target]

model = RandomForestRegressor(n_estimators=100, random_state=42)

model.fit(X_train, y_train)

# 验证集评估

y_pred_val = model.predict(X_val)

print('Validation MAE:', mean_absolute_error(y_val, y_pred_val))

print('Validation RMSE:', np.sqrt(mean_squared_error(y_val, y_pred_val)))

# 6. 预测2024-07-21

day21 = daily[daily['day_index'] == 10].copy()

# 更新Date和day_index到21日

day21['Date'] = pd.to_datetime('2024-07-21').date()

day21['day_index'] = 11

# 计算特征（滚动窗口在group中自动扩展）

day21 = pd.concat([daily, day21]).groupby('BloggerID').apply(rolling_feat).reset_index(drop=True)

day21 = day21[day21['Date'] == pd.to_datetime('2024-07-21').date()]

X21 = day21[features]

day21['pred_follow'] = model.predict(X21)

# 7. 输出前5名结果

top5 = day21[['BloggerID','pred_follow']].sort_values('pred_follow', ascending=False).head(5)

print(top5)