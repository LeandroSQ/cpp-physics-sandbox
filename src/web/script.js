function onBtnFullscreenClick() {
    Module.requestFullscreen(false, true);
}

function onBtnRestartClick() {

}

function onBtnKillClick() {

}

function toggleProgressIndicator(visible = false) {
    const progressIndicator = document.getElementById("progress-indicator");
    if (visible) {
        progressIndicator.classList.remove("hidden");
    } else {
        progressIndicator.classList.add("hidden");
    }
}

function toggleUI(visible = false) {
    const canvas = document.getElementById("canvas");
    const controls = document.getElementById("controls");

    if (visible) {
        canvas.classList.remove("hidden");
        controls.classList.remove("hidden");
    } else {
        canvas.classList.add("hidden");
        controls.classList.add("hidden");
    }
}

function updateProgressIndicator(progress) {
    const progressIndicator = document.getElementById("progress-indicator");
    progressIndicator.value = progress;
    progressIndicator.max = 1.0;
}

function commit(action) {
    if (!document.startViewTransition) {
        return action();
    }

    return document.startViewTransition(action).ready;
}

function showDialog(type, title, message) {
    const dialog = document.getElementById("modal");
    const dialogTitle = dialog.querySelector(".title");
    const dialogMessage = dialog.querySelector(".message");
    const dialogButton = dialog.querySelector("button");
    dialogTitle.innerText = title;
    dialogMessage.innerText = message;

    dialog.classList.remove("is-success", "is-error", "is-warning");
    dialog.classList.add(`is-${type}`);
    dialogButton.classList.remove("is-success", "is-error", "is-warning");
    dialogButton.classList.add(`is-${type}`);


    dialog.showModal();
}

const canvas = document.getElementById("canvas");
canvas.oncontextmenu = function (e) {
    e.preventDefault();
};
canvas.addEventListener("webglcontextlost", function (event) {
    showDialog("error", "Attention!", "WebGL context lost. Please reload the page.");
    event.preventDefault();
}, false);
window.onerror = function (message, source, lineno, colno, error) {
    Module.setStatus("Exception thrown, see JavaScript console");
    showDialog("error", "Attention!", message);
    return true;
};


var Module = {
    canvas,
    totalDependencies: 0,
    // noInitialRun: true,
    monitorRunDependencies(left) {
        this.totalDependencies = Math.max(this.totalDependencies, left);
        toggleProgressIndicator(true);
        updateProgressIndicator((this.totalDependencies - left) / this.totalDependencies);
    },
    onRuntimeInitialized() {
        commit(() => {
            toggleProgressIndicator(false);
            toggleUI(true);
            console.log(Module);
        })
    }
};