from fastapi import FastAPI

from routes import root
from database.sqlite import connection

app = FastAPI()
app.include_router(root)

cursor = connection.cursor()
cursor.executescript('''
    CREATE TABLE IF NOT EXISTS Points (
        point_id    INTEGER PRIMARY KEY AUTOINCREMENT,
        source      VARCHAR(64) NOT NULL,
        group_name  VARCHAR(64) NOT NULL,
        x           BIGINT NOT NULL,
        y           BIGINT NOT NULL
    );
                     
    CREATE INDEX IF NOT EXISTS Points_group_name_idx ON Points (group_name);
''')

connection.commit()
