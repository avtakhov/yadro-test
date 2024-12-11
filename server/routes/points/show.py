import itertools

import fastapi
import pandas as pd
import matplotlib.pyplot as plt

from database.sqlite import connection

router = fastapi.APIRouter()

@router.get('/show')
async def show():
    cursor = connection.cursor()
    cursor.execute('SELECT DISTINCT group_name FROM Points;')
    groups = {group_name: i for i, (group_name, ) in enumerate(cursor.fetchall())}
    cursor.close()

    plt.figure(figsize=(10, 8))
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.title("Plot from table")

    for chunk in pd.read_sql(f"SELECT * FROM Points", connection, chunksize=200):
        for group_name, items in chunk.groupby(chunk.group_name):
            plt.scatter(
                items.x,
                items.y,
                label=group_name,
                c=[groups[group_name]] * len(items),
                cmap='viridis',
                vmin=0,
                vmax=len(groups) - 1
            )
    handles, labels = plt.gca().get_legend_handles_labels()
    unique = dict(zip(labels, handles))
    plt.legend(unique.values(), unique.keys())
    filename = 'show.png'
    plt.savefig(filename, dpi=400)

    return fastapi.responses.FileResponse(filename) 
