import shutil
import os
from doit.action import CmdAction

DOIT_CONFIG = {
    'default_tasks': [],
}

def task_test_esp32():
    """Demo Unit Test ESP32"""

    return {
        'actions': [CmdAction("pio test -vv -e esp32", cwd="./1.UnitTesting")],
        'verbosity': 2,
    }

def task_test_native():
    """Demo Unit Test Native"""

    return {
        'actions': [CmdAction("pio test -vv -e native", cwd="./1.UnitTesting")],
        'verbosity': 2,
    }

