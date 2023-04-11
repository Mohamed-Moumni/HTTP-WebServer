function uploadFile() {
	var fileInput = document.getElementById("file");
	var file = fileInput.files[0];
	var xhr = new XMLHttpRequest();
	xhr.open("POST", "/upload" + encodeURIComponent(file.name));
	xhr.setRequestHeader("Content-Type", file.type);
	xhr.upload.addEventListener("progress", updateProgress);
	xhr.addEventListener("load", uploadComplete);
	xhr.addEventListener("error", uploadFailed);
	xhr.addEventListener("abort", uploadCanceled);
	var fileReader = new FileReader();
	fileReader.onload = function() {
		var arrayBuffer = this.result;
		var blob = new Blob([arrayBuffer], {type: file.type});
		xhr.send(blob);
	};
	fileReader.readAsArrayBuffer(file);
}

function updateProgress(event) {
	var progressPercentage = Math.floor((event.loaded / event.total) * 100);
	var progressBar = document.getElementById("progress-bar");
	progressBar.style.width = progressPercentage + "%";
}

function uploadComplete(event) {
	var message = document.getElementById("message");
	message.innerHTML = "The file was uploaded successfully!";
}

function uploadFailed(event) {
	var message = document.getElementById("message");
	message.innerHTML = "There was an error uploading the file.";
}

function uploadCanceled(event) {
	var message = document.getElementById("message");
	message.innerHTML = "The file upload was canceled.";
}