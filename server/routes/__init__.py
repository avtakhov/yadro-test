import fastapi

from .points import router
from .ping import router

root = fastapi.APIRouter()

root.include_router(points.router)
root.include_router(ping.router)
