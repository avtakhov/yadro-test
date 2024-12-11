import fastapi 

from .add import router
from .clear import router
from .show import router

router = fastapi.APIRouter(prefix='/points')

router.include_router(add.router)
router.include_router(clear.router)
router.include_router(show.router)
