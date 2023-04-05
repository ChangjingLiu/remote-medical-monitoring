import pymysql
# 打开数据库连接
conn_sql = pymysql.connect(host='47.117.167.31', port=3306, user='tj-iothealth',password='6kicwGHJZTfdKchP',db="tj-iothealth",charset='utf8',cursorclass=pymysql.cursors.DictCursor)
# 在上面host中输入你的IP地址，在password中输入你的数据库密码。
print(conn_sql)
print(type(conn_sql))

# 使用 cursor() 方法创建一个 dict 格式的游标对象 cursor
cursor = conn_sql.cursor(pymysql.cursors.DictCursor)
