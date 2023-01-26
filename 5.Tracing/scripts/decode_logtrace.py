import os
import subprocess

Import("env")

logtrace_proc = os.path.join(env.get("PROJECT_PACKAGES_DIR"), "framework-espidf", "tools/esp_app_trace/logtrace_proc.py")
program_path = env.subst("$PROGPATH")

def decode_logtrace(*args, **kwargs):
    trace_filename = env.get("PROGRAM_ARGS")[0]
    # print("Decoding trace ", trace_filename)
    subprocess.Popen("python "+ logtrace_proc + " " + trace_filename + " " + program_path, shell=True)
    # env.Execute("python "+ logtrace_proc + " " + trace_filename + " " + program_path)

env.AddCustomTarget("decode-logtrace", None, decode_logtrace, description="Decode logtrace file")