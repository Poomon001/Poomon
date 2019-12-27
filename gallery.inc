<!--To display the gallery page to both users and editor-->
<?php
session_start();

//login
if (isset($_POST["login"])) {
    $_SESSION["isEditor"] = true;
} else if (isset($_POST['logout'])) {
    $_SESSION["isEditor"] = false;
}//if

//get session data
if (isset($_SESSION["isEditor"])) {
    $isEditor = $_SESSION["isEditor"];
} else {
    $_SESSION["isEditor"] = false;
    $isEditor             = false;
} //if

echo "<nav id='topNav' class='navbar navbar-inverse'>
    <div class='container-fluid'>
        <div class='navbar-header'>";

//editor only buttons
if ($isEditor) {
    
    //logout button
    echo "<form class='navbar-form navbar-right' method='post'>";
    echo "<button type='submit' class= 'btn' name='logout' value='true'>Logout</button>";
    echo "</form>";
    echo "</div>";
    echo "<ul class='nav navbar-nav'>";
    
    //all images
    echo "<li><a href='index.php'>View all</a><li>";
    echo "<li><a href='index.php?view=private'>Private View</a><li>";
    echo "<li><a href='index.php?view=public'>Public View</a><li>";
    echo "<li><a href='index.php?view=approval'>Approval Gallery</a><li>";
    echo "<li><a href='index.php?view=edit'>Edit Gallery</a><li>";
    
} else { //buttons while logged out
    echo "<form class='navbar-form navbar-right' method='post'>";
    echo "<button type='submit' class= 'btn' name='login' value='true'>Login</button>";
    echo "</form>";
    echo "</div>"; 
    echo "<ul class='nav navbar-nav'>";
}//else


//upload form
echo "<li><a href='index.php?page=form'>Upload</a><li>";
echo "</ul>";

//search
echo "<form class='navbar-form navbar-right' method='post'>";
echo "<input type='text' class='form-control' name='search' id='search'>";
echo "<button type='submit' class= 'btn'>
        <span class='glyphicon glyphicon-search'></span>
    </button>";
echo "</form>";

echo "</div>";
echo "</nav>";

//header
echo "<div class='container1'>";
echo "<div class='row'>";
echo "<div id='headerBg' class='col-sm-12'>";
if ($isEditor) {
    
    if (isset($_GET['view'])) {
        
        if ($_GET['view'] == "private") {
            echo "<h1 class='topHead'>Private View</h1>";
        } else if ($_GET['view'] == "public") {
            echo "<h1 class='topHead'>Public View</h1>";
        } else if ($_GET['view'] == "approval") {
            echo "<h1 class='topHead'>Approval View</h1>";
        } else if ($_GET['view'] == "edit") {
            echo "<h1 class='topHead'>Edit View </h1>";
        }
    } else {
        echo "<h1 class='topHead'>Public Private View</h1>";
    } //if
} else {
    echo "<h1 class='topHead'>Public View</h1>";
} //if
echo "</div>";//sm-12
echo "</div>";//row
echo " </div>";//container

//call Json
$file = "galleryinfo.json";
$json = file($file);
$jsonstring = "";

//read json
foreach ($json as $line) {
    $jsonstring .= $line;
}//foreach

//decode json into array
$phparray = json_decode($jsonstring, true);
$dh = opendir('uploadedimages/');

//download all images
if (isset($_POST["downloadall"])) {
    $zipname = "uploadedimages/images.zip";
    $zip     = new ZipArchive;
    $zip->open($zipname, ZIPARCHIVE::CREATE | ZIPARCHIVE::OVERWRITE); //create zip, if exists overwrite it
    
    //iterate through each file in json
    for ($i = 0; $i < sizeof($phparray); $i++) {
        $fileName = $phparray[$i]['imageFile'];
        $zip->addFile("uploadedimages/" . $fileName);
    }
    $zip->close(); //create zip
    
    //download
    header('Content-Type: application/zip');
    header('Content-disposition: attachment; filename=images.zip');
    header('Content-Length: ' . filesize($zipname));
    ob_clean();
    flush();
    readfile($zipname);
}//if

//edit single image's json data
if (isset($_POST["editSingle"])) {
    for ($i = 0; $i < sizeof($phparray); $i++) {
        for ($j = 0; $j < sizeof($phparray); $j++) {
            if ($phparray[$i]['UID'] == $_POST['editSingle']) {
                
                if ($_POST['fnameSingle'] != "") {
                    $phparray[$i]['firstname'] = $_POST['fnameSingle'];
                }
                if ($_POST['lnameSingle'] != "") {
                    $phparray[$i]['lastname'] = $_POST['lnameSingle'];
                }
                if ($_POST['descSingle'] != "") {
                    $phparray[$i]['description'] = $_POST['descSingle'];
                }
                if ($_POST['tagsSingle'] != "") {
                    $phparray[$i]['tags'] = $_POST['tagsSingle'];
                }
                
                $jsoncode = json_encode($phparray, JSON_PRETTY_PRINT);
                file_put_contents($file, $jsoncode);
                break;
            }//if
        }//for
    }//for
}//if

/*used to be checkedEdit*/
if (isset($_POST['checkedEdit'])) {
    for ($i = 0; $i < sizeof($_POST['checkedEdit']); $i++) {
        for ($j = 0; $j < sizeof($phparray); $j++) {
            if ($phparray[$j]['imageFile'] == $_POST['checkedEdit'][$i]) {
                $fnamepost = "fname" . $_POST['checkedEdit'][$i];
                $fnamepost = str_replace(".", "_", $fnamepost);
                
                $lnamepost = "lname" . $_POST['checkedEdit'][$i];
                $lnamepost = str_replace(".", "_", $lnamepost);
                
                $descpost = "desc" . $_POST['checkedEdit'][$i];
                $descpost = str_replace(".", "_", $descpost);
                
                $tagspost = "tags" . $_POST['checkedEdit'][$i];
                $tagspost = str_replace(".", "_", $tagspost);
                $phparray[$j]['firstname'] = $_POST[$fnamepost];
                $phparray[$j]['lastname'] = $_POST[$lnamepost];
                $phparray[$j]['description'] = $_POST[$descpost];
                $phparray[$j]['tags'] = $_POST[$tagspost];
                
                $jsoncode = json_encode($phparray, JSON_PRETTY_PRINT);
                file_put_contents($file, $jsoncode);
                break;
            }//if
        }//for
    }//for
} //if        

//if delete button is pressed
if (isset($_POST["delete"]) && !empty($_POST["checked"])) {
    //iterate through each checked box and delete from server
    for ($i = 0; $i < sizeof($_POST["checked"]); $i++) {
        if (file_exists("uploadedimages/" . $_POST["checked"][$i])) {
            unlink("uploadedimages/" . $_POST["checked"][$i]);
        }//if
        if (file_exists("uploadedimages/thumbnails/" . $_POST["checked"][$i])) {
            unlink("uploadedimages/thumbnails/" . $_POST["checked"][$i]);
        }//if
        
        //iterate through json and delete all json entries that match the checked boxes
        for ($a = 0; $a < sizeof($phparray); $a++) {
            for ($b = 0; $b < sizeof($phparray[$a]); $b++) {
                if ($phparray[$a]['imageFile'] == $_POST["checked"][$i]) {
                    unset($phparray[$a]);
                    $phparray = array_values($phparray);
                    $jsoncode = json_encode($phparray, JSON_PRETTY_PRINT);
                    file_put_contents($file, $jsoncode);
                    break;
                } //if
            } //for
        } //for
    } //for
} //if

//if approve button is pressed
if (isset($_POST["approve"]) && !empty($_POST["checked"])) {
    for ($i = 0; $i < sizeof($_POST["checked"]); $i++) {
        //iterate through json and delete all json entries that match the checked boxes
        for ($j = 0; $j < sizeof($phparray); $j++) {
            for ($k = 0; $k < sizeof($phparray[$j]); $k++) {
                if ($phparray[$j]['imageFile'] == $_POST["checked"][$i]) {
                    $phparray[$j]['approved'] = 'true';
                    $phparray                 = array_values($phparray);
                    $jsoncode                 = json_encode($phparray, JSON_PRETTY_PRINT);
                    file_put_contents($file, $jsoncode);
                    break;
                } //if
            } //for
        } //for
    } //for
} //if

echo "<br><div class='row'>";
echo "<div class='col-sm-2'>";
//sort buttons
echo "<form method='post'>";
echo "<button type='submit' class='btn btn-primary' id='dSort' name='sort' value='date'>Sort By Date</button><br><br>";
echo "<button type='submit' class='btn btn-primary' id='fSort' name='sort' value='fname'>Sort By First Name</button><br><br>";
echo "<button type='submit' class='btn btn-primary' id='lSort' name='sort' value='lname'>Sort By Last Name</button><br><br>";
echo "</form>";

//download all button
if ($isEditor) {
    echo "<form method='post'>";
    echo "<button type='submit' id='downloadAll' class='btn btn-success' name='downloadall' value='downloadall'>Download All</button><br><br>";
    echo "</form>";
}//if

echo "<form method='post'>"; //open gallery form (contains checkboxes)
if ($isEditor) {
    if (isset($_GET['view']) && $_GET['view'] == "approval") {
        echo "<button type='submit' id='approve' class='btn btn-warning button' name='approve' value='approve'>Approve</button><br><br>";
    }//if
    echo "<button type='submit' id='deleteSelectedimage' class='btn btn-danger' name='delete' value='delete'>Delete</button><br><br>";
}//if

//edit
if ($isEditor && isset($_GET['view']) && $_GET["view"] == "edit") {
    echo "<button type='submit' id='editMultiple' class='btn btn-secondary' name='editMulti' value='true'>Edit</button>";
}//if
echo "</div>"; //sm-2




//container for thumbnails
echo "<div id='body1' class='col-sm-10'>";

if (isset($_POST['sort'])) {
    if ($_POST['sort'] == "date") {
		
        function custom_sort($a, $b){
            return $a['UID'] > $b['UID'];
        }//custom_sort
        usort($phparray, "custom_sort");
		
    } else if ($_POST['sort'] == "lname") {
		
        function compareByName($a, $b){
            return strcmp(strtolower($a["lastname"]), strtolower($b["lastname"]));
        }//compareByName
        usort($phparray, 'compareByName');
		
    } else if ($_POST['sort'] == "fname") {
		
        function compareByName($a, $b){
            return strcmp(strtolower($a["firstname"]), strtolower($b["firstname"])); 
        }//compareByName
        usort($phparray, 'compareByName');
		
    }//else
}//fi

$count = 0;//store number 

//check if user is editor
if ($isEditor) {
    for ($l = 0; $l < sizeof($phparray); $l++) {
        $fileName = $phparray[$l]['imageFile']; //get file name from json
        $fileLoc  = 'uploadedimages/' . $fileName; //file location
        $thumbLoc = 'uploadedimages/thumbnails/' . $fileName; //thumbnail location
        
        //aproval gallery
        if (isset($_GET['view']) && $_GET["view"] == "approval") {
            //view unapproved images only
            if (isset($_POST["search"]) && $_POST["search"] != "") { //if user is searching
                if ($phparray[$l]["approved"] == "false") {
                    $currentTags = strtolower($phparray[$l]['tags']); //turn tags from json into array
                    $currentFirstName = strtolower($phparray[$l]['firstname']);
                    $currentLastName = strtolower($phparray[$l]['lastname']);
                    $currentDesc = strtolower($phparray[$l]['description']);
                    $searchTerm = strtolower($_POST['search']); //user input
                    
                    
                    
                    foreach ($phparray as $line) {
                        //show private images only
                        if (strpos(trim($currentTags), trim($searchTerm)) !== false 
							|| strpos(trim($currentFirstName), trim($searchTerm)) !== false
							|| strpos(trim($currentLastName), trim($searchTerm)) !== false
							|| strpos(trim($currentDesc), trim($searchTerm)) !== false
							&& $phparray[$l]["approved"] == "false") {
                            echo "<div class='col-sm-2 col-xs-4'>";
                            echo "<input  type='checkbox' class='checkBox' name='checked[]' value='$fileName'>";
                            echo "<div class='thumbnail'>";
                            echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id='i$count'>";
                            echo "<img src=" . $thumbLoc . " alt='thumb'>";
                            echo "</a>";
                            echo "</div>";
                            echo "</div>";
                            $count++;
                            break;
                        }//if
                    }//foreach
                }//if
            } else if ($phparray[$l]["approved"] == "false") {
                echo "<div class='col-sm-2 col-xs-4'>";
                echo "<input  type='checkbox' class='checkBox' name='checked[]' value='$fileName'>";
                echo "<div class='thumbnail'>";
                echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id='i$count'>";
                echo "<img src=" . $thumbLoc . " alt='thumb'>";
                echo "</a>";
                echo "</div>";
                echo "</div>";
                $count++;
            }//else
        } else if (isset($_GET['view']) && $_GET["view"] == "edit") { //edit view
            $firstname = $phparray[$l]['firstname'];
            $lastname  = $phparray[$l]['lastname'];
            $desc = $phparray[$l]['description'];
            $tags = $phparray[$l]['tags'];


            //view all images
            if (isset($_POST["search"]) && $_POST['search'] != "") { //if user is searching
                if ($phparray[$l]["approved"] == "true") {
                    
                    $currentTags = strtolower($phparray[$l]['tags']); //turn tags from json into array
                    $currentFirstName = strtolower($phparray[$l]['firstname']);
                    $currentLastName = strtolower($phparray[$l]['lastname']);
                    $currentDesc = strtolower($phparray[$l]['description']);
                    $searchTerm = strtolower($_POST['search']); //user input
                    foreach ($phparray as $line) {
                        
                        if (strpos(trim($currentTags), trim($searchTerm)) !== false 
							|| strpos(trim($currentFirstName), trim($searchTerm)) !== false 
							|| strpos(trim($currentLastName), trim($searchTerm)) !== false 
							|| strpos(trim($currentDesc), trim($searchTerm)) !== false 
							&& $phparray[$l]["approved"] == "true") {
                            echo "<div class='col-sm-2 col-xs-4'>"; //added
                            echo "<input type='checkbox' class='checkBox' value='$fileName' name='checked[]' >";
                            echo "<input type='checkbox' name='checkedEdit[]' value='$fileName' onclick='changeVisibility(\"$count\")'>";
                            echo "<div class='thumbnail'>";
                            echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id='i$count'>";
                            echo "<img src=" . $thumbLoc . " alt='thumb'>";
                            echo "</a>";
                            echo "</div>";
                            
                            echo "<div class='hidden' id='$count'>
                                    <h4>First name</h4><br><input type='text' name='fname$fileName' value=$firstname><br>
                                    <h4>Last name</h4><br><input type='text' name='lname$fileName' value=$lastname><br>
                                    <h4>Description</h4><br><input type='text' name='desc$fileName' value=$desc><br>
                                    <h4>Tags<br></h4><input type='text' name='tags$fileName' value=$tags>
                                </div>";
                            echo "</div>"; //added
                            $count++;
                            break;
                        }//if
                    }//foreach
                }//if
            } else if ($phparray[$l]["approved"] == "true") {
                echo "<div class='col-sm-2 col-xs-4'>"; //added
                                                
                echo "<input  type='checkbox' class='checkBox' value='$fileName' name='checked[]'>";
                echo "<input type='checkbox' name='checkedEdit[]' value='$fileName' onclick='changeVisibility(\"$count\")'>";
                echo "<div class='thumbnail'>";
                echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id='i$count'>";
                echo "<img src=" . $thumbLoc . " alt='thumb'>";
                echo "</a>";
                echo "</div>";
                //here is the bugs
                echo "<div class='hidden' id='$count'>
                            <h4>First name</h4><br><input type='text' name='fname$fileName' value='$firstname'><br>
                            <h4>Last name</h4><br><input type='text' name='lname$fileName' value='$lastname'><br>
                            <h4>Description</h4><br><input type='text' name='desc$fileName' value='$desc'><br>
                            <h4>Tags<br></h4><input type='text' name='tags$fileName' value='$tags'>
                        </div>";
                echo "</div>"; //added
                $count++;
            }//else
        } else {
            //view all images
            if (isset($_GET["view"]) && $_GET["view"] == "private") { //private only 
                if (isset($_POST["search"]) && $_POST["search"] != "") {
                    if ($phparray[$l]["approved"] == "true" && $phparray[$l]["access"] == "private") {
                        $currentTags      = strtolower($phparray[$l]['tags']); //turn tags from json into array
                        $currentFirstName = strtolower($phparray[$l]['firstname']);
                        $currentLastName  = strtolower($phparray[$l]['lastname']);
                        $currentDesc      = strtolower($phparray[$l]['description']);
                        $searchTerm       = strtolower($_POST['search']); //user input
                        foreach ($phparray as $line) {
                            //show private images only
                            if (strpos(trim($currentTags), trim($searchTerm)) !== false 
								|| strpos(trim($currentFirstName), trim($searchTerm)) !== false 
								|| strpos(trim($currentLastName), trim($searchTerm)) !== false 
								|| strpos(trim($currentDesc), trim($searchTerm)) !== false 
								&& $phparray[$l]["access"] == "private" 
								&& $phparray[$l]["approved"] == "true") {
                                echo "<div class='col-sm-2 col-xs-4'>";
                                echo "<input id = 'checkbox' type='checkbox' class='checkBox' name='checked[]' value='$fileName'>";
                                echo "<div class='thumbnail'>";
                                echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id = 'i$count'>";
                                echo "<img src=" . $thumbLoc . " alt='thumb'>";
                                echo "</a>";
                                echo "</div>";
                                echo "</div>";
                                $count++;
                                break;
                            }//if
                        }//foreach
                    }//if
                } else if ($phparray[$l]["access"] == "private" && $phparray[$l]["approved"] == "true") { //show all private images if user isnt searching
                    echo "<div class='col-sm-2 col-xs-4'>";
                    echo "<input  type='checkbox' class='checkBox' name='checked[]' value='$fileName'>";
                    echo "<div class='thumbnail'>";
                    echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id='i$count'>";
                    echo "<img src=" . $thumbLoc . " alt='thumb'>";
                    echo "</a>";
                    echo "</div>";
                    echo "</div>";
                    $count++;
                }//else
            } else if (isset($_GET["view"]) == "public") { 
				//public  only 
                if (isset($_POST["search"]) && $_POST["search"] != "") {
                    if ($phparray[$l]["approved"] == "true" && $phparray[$l]["access"] == "public") {
                        $currentTags      = strtolower($phparray[$l]['tags']); //turn tags from json into array
                        $currentFirstName = strtolower($phparray[$l]['firstname']);
                        $currentLastName  = strtolower($phparray[$l]['lastname']);
                        $currentDesc      = strtolower($phparray[$l]['description']);
                        $searchTerm       = strtolower($_POST['search']); //user input
                        foreach ($phparray as $line) {
                            if (strpos(trim($currentTags), trim($searchTerm)) !== false || strpos(trim($currentFirstName), trim($searchTerm)) !== false || strpos(trim($currentLastName), trim($searchTerm)) !== false || strpos(trim($currentDesc), trim($searchTerm)) !== false && $phparray[$l]["access"] == "public" && $phparray[$l]["approved"] == "true") {
                                echo "<div class='col-sm-2 col-xs-4'>";
                                echo "<input id = 'checkbox' type='checkbox' class='checkBox' name='checked[]' value='$fileName'>";
                                echo "<div class='thumbnail'>";
                                echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id='i$count'>";
                                echo "<img src=" . $thumbLoc . " alt='thumb'>";
                                echo "</a>";
                                echo "</div>";
                                echo "</div>";
                                $count++;
                                break;
                            }//if
                        }//foreach
                    }//if
                } else if ($phparray[$l]["access"] == "public" && $phparray[$l]["approved"] == "true") {
                    echo "<div class='col-sm-2 col-xs-4'>";
                    echo "<input id = 'checkbox' type='checkbox' class='checkBox' name='checked[]' value='$fileName'>";
                    echo "<div class='thumbnail'>";
                    echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id='i$count'>";
                    echo "<img src=" . $thumbLoc . " alt='thumb'>";
                    echo "</a>";
                    echo "</div>";
                    echo "</div>";
                    $count++;
                }//else
            } else { 
				//view public and private images
                if (isset($_POST["search"]) && $_POST["search"] != "") { //if user is seraching
                    if ($phparray[$l]["approved"] == "true") {
                        $currentTags      = strtolower($phparray[$l]['tags']); //turn tags from json into array
                        $currentFirstName = strtolower($phparray[$l]['firstname']);
                        $currentLastName  = strtolower($phparray[$l]['lastname']);
                        $currentDesc      = strtolower($phparray[$l]['description']);
                        $searchTerm       = strtolower($_POST['search']); //user input
                        
                        foreach ($phparray as $line) {
                            if (strpos(trim($currentTags), trim($searchTerm)) !== false 
								|| strpos(trim($currentFirstName), trim($searchTerm)) !== false 
								|| strpos(trim($currentLastName), trim($searchTerm)) !== false 
								|| strpos(trim($currentDesc), trim($searchTerm)) !== false 
								&& $phparray[$l]["approved"] == "true") {
                                echo "<div class='col-sm-2 col-xs-4'>";
                                echo "<input type='checkbox' class='checkBox' name='checked[]' value='$fileName'>";
                                echo "<div class='thumbnail'>";
                                echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id = 'i$count'>";
                                echo "<img src=" . $thumbLoc . " alt='thumb'>";
                                echo "</a>";
                                echo "</div>";
                                echo "</div>";
                                $count++;
                                break;
                            } //if
                        } //for
                    }//if
                } else { 
					//view all images
                    if ($phparray[$l]["approved"] == "true") {
                        echo "<div class='col-sm-2 col-xs-4'>";
                        echo "<input type='checkbox' class='checkBox' name='checked[]' value='$fileName'>";
                        echo "<div class='thumbnail'>";
                        echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id='i$count'>";
                        echo "<img src=" . $thumbLoc . " alt='thumb'>";
                        echo "</a>";
                        echo "</div>";
                        echo "</div>";
                        $count++;
                    }//if
                } //else
            } //else
        } //else
    } //for
} else { 
	//public gallery (non-editor)
    //iterate through json
    for ($l = 0; $l < sizeof($phparray); $l++) {
        $fileName = $phparray[$l]['imageFile'];
        $fileLoc  = 'uploadedimages/' . $fileName;
        $thumbLoc = 'uploadedimages/thumbnails/' . $fileName;
        
        if (isset($_POST["search"]) && $_POST["search"] != "") {
            if ($phparray[$l]["approved"] == "true" && $phparray[$l]["access"] == "public") {
                $currentTags      = strtolower($phparray[$l]['tags']); //turn tags from json into array
                $currentFirstName = strtolower($phparray[$l]['firstname']);
                $currentLastName  = strtolower($phparray[$l]['lastname']);
                $currentDesc      = strtolower($phparray[$l]['description']);
                $searchTerm       = strtolower($_POST['search']); //user input
                
                
                
                foreach ($phparray as $line) {
                    if (strpos(trim($currentTags), trim($searchTerm)) !== false || strpos(trim($currentFirstName), trim($searchTerm)) !== false || strpos(trim($currentLastName), trim($searchTerm)) !== false || strpos(trim($currentDesc), trim($searchTerm)) !== false && $phparray[$l]["access"] == "public" && $phparray[$l]["approved"] == "true") {
                        echo "<div class='col-sm-2 col-xs-4'>";
                        echo "<div class='thumbnail'>";
                        echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id='i$count'>";
                        echo "<img src=" . $thumbLoc . " alt='thumb'>";
                        echo "</a>";
                        echo "</div>";
                        echo "</div>";
                        $count++;
                        break;
                    }//if
                }//foreach
            }//if
        } else if ($phparray[$l]["access"] == "public" && $phparray[$l]["approved"] == "true") {
            echo "<div class='col-sm-2 col-xs-4'>";
            echo "<div class='thumbnail'>";
            echo "<a href= 'javascript:displayLightBox(" . "\"" . $fileLoc . "\", $count)' id='i$count'>";
            echo "<img src=" . $thumbLoc . " alt='thumb'>";
            echo "</a>";
            echo "</div>";
            echo "</div>";
            $count++;
        }//else
    }//for
}//if

//close container-row
echo "</div>"; //row
echo "</form>";
echo "</div>"; //row

include "lightbox.inc"; //lightbox html

closedir($dh);
?>

