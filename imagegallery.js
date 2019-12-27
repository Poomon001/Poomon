/*To display the lightbox with description and arrows*/
var width = 0; // big image width
var searchresults = [];
var name = "";
var i;
var xmlhttp = new XMLHttpRequest();
var url = "galleryinfo.json";

xmlhttp.onreadystatechange = function () {
	if (this.readyState == 4 && this.status == 200) {
		var myArr = JSON.parse(this.responseText);

		//get contents of all images stored in array - searchresults
		for (i = 0; i < myArr.length; i++) {
			searchresults.push(myArr[i]); //all images
		}
	}
};

xmlhttp.open("GET", url, true);
xmlhttp.send();

//display lightbox and do a calculation when the image is shifted (arrow button)
function displayLightBox(imgSrc, imgid) {

	if (imgSrc == "none" && imgid == -1) {
		document.getElementById("lightbox").className = "hidden";
		document.getElementById("boundaryBigImage").className = "hidden";
	} else {
		var image = new Image(); // preload large image

		// access div tag with big image
		var bigImage = document.getElementById("bigImage");

		image.src = imgSrc;

		//show lightbox
		document.getElementById("lightbox").className = "unhidden";
		document.getElementById("boundaryBigImage").className = "unhidden";

		//next image button
		if (document.getElementById("i" + (imgid + 1)) != null) {
			document.getElementById("next").href = document.getElementById("i" + (imgid + 1).toString()).getAttribute("href");
		}//if
		//previous button
		if (document.getElementById("i" + (imgid - 1)) != null) {
			document.getElementById("previous").href = document.getElementById("i" + (imgid - 1).toString()).getAttribute("href");
		}//if

		var a = document.getElementById("i" + (imgid).toString()).getAttribute("href");
		a = a.substring(a.indexOf("uploadedimages"), a.lastIndexOf("\""));
		document.getElementById("download").download = a.substring(a.indexOf("/") + 1, a.length); //name of file
		document.getElementById("download").href = a; // url to download from (uploadedimages/FILENAME)

		//force bigImage to preload to get width and center it
		image.onload = function () {
			//width = image.width * 0.35;
			width = image.width;
			document.getElementById("boundaryBigImage").style.width = width + "px";
		}//anonymous function

		//put big image into page
		bigImage.src = image.src;

		for (i = 0; i < searchresults.length; i++) {
			//file name matches source file name
			if (searchresults[i].imageFile == imgSrc.substring(imgSrc.lastIndexOf("/") + 1, imgSrc.length)) {
				//first name + last name
				name = searchresults[i].firstname;
				name += " " + searchresults[i].lastname;

				//show first name and description in lightbox
				document.getElementById("name").innerHTML = name;
				document.getElementById("photoDesc").innerHTML = searchresults[i].description;

				//show default values in lightbox edit input form
				document.getElementById("fnameSingle").value = searchresults[i].firstname;
				document.getElementById("lnameSingle").value = searchresults[i].lastname;
				document.getElementById("descSingle").value = searchresults[i].description;
				document.getElementById("tagsSingle").value = searchresults[i].tags;
				document.getElementById("editSingle").value = searchresults[i].UID;
				break;
			}//if
		}//for
	}//else
}//displayLightBox

//change visibility of div id
function changeVisibility(divID) {
	var element = document.getElementById(divID);

	//if element exists
	if (element) {
		element.className = (element.className == "hidden") ? "unhidden" : "hidden";
	}//if
}//changeVisibility