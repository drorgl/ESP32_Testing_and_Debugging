import os
import sys

Import("env")

def runNativeChecks(source, target, env):
    project_name = os.path.basename(os.path.normpath(env['PROJECT_DIR']))
    if ('PIOTEST_RUNNING_NAME' in env):
        project_name = project_name + " - " + env['PIOTEST_RUNNING_NAME']
    sys.stderr.write("Project name: {}\r\n".format(project_name))

env.AddPostAction("$BUILD_DIR/$PROGNAME$PROGSUFFIX", runNativeChecks)