import fastapi
import pydantic
import sqlite3

from database.sqlite import connection

router = fastapi.APIRouter()


@router.post('/clear')
async def clear():
    cursor = connection.cursor()
    cursor.execute(
        'DELETE FROM Points;',
    )
    connection.commit()
