import pytermgui as ptg
import subprocess
import re
import os


def model_select_handler(label_text):

    label_item = ptg.get_widget("model-name")

    default_text = "Selected Model : "

    label_item.set_style("value", ptg.MarkupFormatter(default_text + label_text))

    popup_modelsel.close()


def get_config_values():

    label_item = ptg.get_widget("model-name")
    model = label_item.get_lines()

    checkbox_preview = ptg.get_widget("checkbox-preview").checked

    checkbox_preview_text = "1" if checkbox_preview is True else "0"

    input_roiA = ptg.get_widget("input-roiA").value
    input_roiB = ptg.get_widget("input-roiB").value

    input_roi_nightA = ptg.get_widget("input-night-roiA").value
    input_roi_nightB = ptg.get_widget("input-night-roiB").value

    string_param = (
        "../build/realtime-counting --model_path="
        + "../dnn_files/"
        + "".join(model).replace("Selected Model : ", "")
        + " --class_path=../dnn_files/coco_c.names"
        + " --model_size=640"
        + " --roi_A="
        + "".join(input_roiA).replace(" ", "")
        + " --roi_B="
        + "".join(input_roiB).replace(" ", "")
        + " --night_roi_1="
        + "".join(input_roi_nightA).replace(" ", "")
        + " --night_roi_2="
        + "".join(input_roi_nightB).replace(" ", "")
        + " --preview="
        + checkbox_preview_text
    )

    label_param.set_style("value", ptg.MarkupFormatter(markup=string_param))

    label_param.parent_align = 3
    label_param.padding = 2

    namespace.MainWindow.height = org_height + 3


def start_rvc():

    rvc_env = os.environ.copy()

    rvc_env["OPENCV_LOG_LEVEL"] = "OFF"

    string_param = "".join(label_param.get_lines())
    ansi_escape = re.compile(r"(\x9B|\x1B\[)[0-?]*[ -\/]*[@-~]")
    string_param_new = ansi_escape.sub("", string_param)
    params = string_param_new.split(" ")

    params_new = [item for item in params if item]

    process = subprocess.Popen(
        params_new,
        # close_fds=False,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        env=rvc_env,
    )

    stdout_string = ""
    stdout_lines = process.stdout.readlines()
    for stdout_line in stdout_lines:
        stdout_string = stdout_string + stdout_line.decode() + "\n"

    label_output.set_style(
        "value",
        ptg.MarkupFormatter(
            markup=stdout_string,
            ensure_reset=True,
            ensure_strip=True,
        ),
    )

    stdout_string = ""

    namespace.MainWindow.height = org_height + len(stdout_lines)
    manager.alert(str(len(stdout_lines)))
    label_output.parent_align = 3
    label_output.padding = 2


with open("namespace.yaml", "r") as ptg_file:
    namespace = ptg.YamlLoader().load(ptg_file)

with ptg.WindowManager() as manager:

    string_param = ""

    manager.add(namespace.MainWindow.center())
    namespace.MainWindow.set_title("Realtime Vehicle Counting Configuration")
    # namespace.MainWindow.height = namespace.MainWindow.height + 10
    org_height = namespace.MainWindow.height + 1
    org_width = namespace.MainWindow.width

    popup_modelsel = namespace.ModelSelection.center()
    namespace.ModelSelection.set_title("Model Selection")
    popup_modelsel.bind(ptg.keys.ESC, lambda window, _: window.close())

    # label_model = ptg.get_widget("model-name")
    button_model = ptg.get_widget("select-model")
    button_model.onclick = lambda *_: manager.add(popup_modelsel)

    button_nano = ptg.get_widget("set-nano")
    button_nano.onclick = lambda *_: model_select_handler("yolov5n.onnx")
    button_nano_opt = ptg.get_widget("set-nano-opt")
    button_nano_opt.onclick = lambda *_: model_select_handler("yolov5n-opt.onnx")
    button_small = ptg.get_widget("set-small")
    button_small.onclick = lambda *_: model_select_handler("yolov5s-opt.onnx")

    label_param = ptg.get_widget("label-param")
    label_output = ptg.get_widget("label-output")

    button_set = ptg.get_widget("set-config")
    button_set.onclick = lambda *_: get_config_values()

    button_start = ptg.get_widget("start-app")
    button_start.onclick = lambda *_: start_rvc()

    button_quit = ptg.get_widget("quit-app")
    button_quit.onclick = lambda *_: manager.exit()

    manager.run()
