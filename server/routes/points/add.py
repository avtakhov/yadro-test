import fastapi
import pydantic

from database.sqlite import connection

router = fastapi.APIRouter()

class Point(pydantic.BaseModel):
    source: str
    group: str | int
    x: int
    y: int

class RequestData(pydantic.BaseModel):
    points: list[Point]


@router.post('/add')
async def add(
    data: RequestData,
):
    cursor = connection.cursor()
    cursor.executemany(
        'INSERT INTO Points (source, group_name, x, y) VALUES (?, ?, ?, ?)',
        [(p.source, str(p.group), p.x, p.y) for p in data.points]
    )
    connection.commit()
