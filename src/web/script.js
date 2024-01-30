/* UI Event listeners */
function onBtnFullscreenClick() {
    Module.requestFullscreen(false, true);
}

function onBtnRestartClick() {
    window.location.reload();
}

function onBtnKillClick() {
    Module.pauseMainLoop();
}

/* UI Helpers */
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

/* Utility functions */
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

function onLoadError(resource) {
    console.trace();
    console.error(`Failed to load resource: ${resource}`);
    showDialog("error", "Attention!", `Failed to load resource: ${resource}`);
}

function resize() {
    // Check if currently in fullscreen mode
    if (document.fullscreenElement !== null) return;
    const container = canvas.parentElement;

    const width = container.clientWidth;
    const height = container.clientHeight;

    if (Module && "setCanvasSize" in Module) {
        console.trace(`Resizing canvas to ${width}x${height}`);
        setTimeout(() => {
            Module.setCanvasSize(width, height, false);
        }, 25);
    }
}

/* Setup canvas */
const canvas = document.getElementById("canvas");
canvas.oncontextmenu = function (e) {
    e.preventDefault();
};
canvas.addEventListener("webglcontextlost", function (event) {
    showDialog("error", "Attention!", "WebGL context lost. Please reload the page.");
    event.preventDefault();
}, false);

/* Setup window sizing */
window.addEventListener("resize", function (event) {
    resize();
}, false);
window.addEventListener("orientationchange", function (event) {
    resize();
});

/* Setup error handling */
window.addEventListener("error", function (event) {
    event.preventDefault();
    onLoadError(event.target.src);
}, true);
window.onerror = function (message, source, lineno, colno, error) {
    showDialog("error", "Attention!", message);
    return true;
};

/* Setup emscripten module */
var Module = {
    canvas,
    totalDependencies: 0,
    noInitialRun: false,
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
        }).then(() => {
            resize();
        });
    },
    onAbort() {
        showDialog("error", "Attention!", "The program has aborted.");
    },
    onExit() {
        showDialog("success", "Success!", "The program has exited.");
    },
    preRun() {
        toggleProgressIndicator(true);
        // resize();
    },
};