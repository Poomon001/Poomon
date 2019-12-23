<!--to store the form data-->
<?php
include "header.inc";

if (isset($_GET["page"]) && $_GET["page"] == "form") {
    
    $fnameErr = $lnameErr = $uploadErr = $descErr = $tagErr = $publicprivateErr = $copyrightErr = "";//show error
    $fname = $lname = $upload = $desc = $tag = $copyright = $privateAccess = $access = "";//store input data
    $publicAccess = "checked"; //default public to checked
    $uploaded = false; //if the file was successfully uploaded
    
    $target_dir = "uploadedimages/"; //folder for images
    $uploadOk = 1; //if ok to upload
    $UID = str_pad(file_get_contents('identifier.txt'), 4, '0', STR_PAD_LEFT); //get UID
    
	//show errors
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        
        if (empty(trim($_POST["fname"]))) {
            $fnameErr = "First name is required";
        } else {
            $fname = test_input($_POST["fname"]);
        }//if
        
        if (empty(trim($_POST["lname"]))) {
            $lnameErr = "Last name is required";
        } else {
            $lname = test_input($_POST["lname"]);
        }//if
        
        if (empty($_FILES['upload']['tmp_name'])) {
            $uploadErr = "You must upload a file";
        } else {
            $target_file   = $target_dir . basename($_FILES["upload"]["name"]);
            $imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));
            
            $check = getimagesize($_FILES["upload"]["tmp_name"]);
            if ($check == false) {
                $uploadErr = "The file you uploaded is not an image";
                $uploadOk  = 0;
            } else if ($_FILES["upload"]["size"] > 4000000) {
                $uploadErr = "The file you uploaded is too large";
                $uploadOk  = 0;
            } else if ($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg") {
                $uploadErr = "Only JPG, JPEG, and PNG files are allowed";
                $uploadOk  = 0;
            }//else
            
            if ($uploadOk == 1) {
                //upload file and name it with its UID
                if (move_uploaded_file($_FILES["upload"]["tmp_name"], $target_dir . $UID . "." . $imageFileType) == false) {
                    $uploadErr = "There was an error uploading your image";
                    $uploaded  = false;
                } else {
                    $uploaded = true;
                    
                    if ($imageFileType == "jpg" || $imageFileType == "jpeg") {
                        $source_img = @imagecreatefromjpeg($target_dir . $UID . "." . $imageFileType);
                    } else if ($imageFileType == "png") {
                        $source_img = @imagecreatefrompng($target_dir . $UID . "." . $imageFileType);
                    }//if
                    
                    if (!$source_img) {
                        echo "error";
                    }//if
                    $new_w = 200;
                    $new_h = 200;
                    
                    $orig_w = imagesx($source_img);
                    $orig_h = imagesy($source_img);
                    
                    $w_ratio = ($new_w / $orig_w);
                    $h_ratio = ($new_h / $orig_h);
                    
                    if ($orig_w > $orig_h) { //landscape
                        $crop_w = round($orig_w * $h_ratio);
                        $crop_h = $new_h;
                        $src_x  = ceil(($orig_w - $orig_h) / 2);
                        $src_y  = 0;
                    } else if ($orig_w < $orig_h) { //portrait
                        $crop_h = round($orig_h * $w_ratio);
                        $crop_w = $new_w;
                        $src_x  = 0;
                        $src_y  = ceil(($orig_h - $orig_w) / 2);
                    } else { //square
                        $crop_w = $new_w;
                        $crop_h = $new_h;
                        $src_x  = 0;
                        $src_y  = 0;
                    }//else
                    $dest_img = imagecreatetruecolor($new_w, $new_h);
                    imagecopyresampled($dest_img, $source_img, 0, 0, $src_x, $src_y, $crop_w, $crop_h, $orig_w, $orig_h);
                    
                    if ($imageFileType == "jpg" || $imageFileType == "jpeg") {
                        if (imagejpeg($dest_img, "uploadedimages/thumbnails/" . $UID . "." . $imageFileType)) {
                            imagedestroy($dest_img);
                            imagedestroy($source_img);
                        } else {
                            echo "error";
                        }//if
                    } else if ($imageFileType == "png") {
                        if (imagepng($dest_img, "uploadedimages/thumbnails/" . $UID . "." . $imageFileType)) {
                            imagedestroy($dest_img);
                            imagedestroy($source_img);
                        } else {
                            echo "error";
                        }//else
                    }//else
                }//else
            }//if
        }//else
        
		//error when there is no photo
        if (empty($_POST["desc"])) {
            $descErr = "You must add a description to your photo";
        } else {
            $desc = test_input($_POST["desc"]);
        }//if
		
        //error of no tag
        if (empty($_POST["tag"])) {
            $tagErr = "You must add tags to your photo";
        } else {
            $tag = test_input($_POST["tag"]);
        }//if
		
		//error of no access
        if (empty($_POST["access"])) {
            $accessErr = "You must add access to your photo";
        } else {
            $access = test_input($_POST["access"]);
        }//if
		
        //error of no copyright
        if (empty($_POST["copyright"])) {
            $copyrightErr = "You must agree to the copyright notice";
        } else {
            $copyright = test_input($_POST["copyright"]);
        }//if
		
		//reset
        if (isset($_POST["reset"])) {
            $fnameErr = $lnameErr = $uploadErr = $descErr = $tagErr = $copyrightErr = "";
            $fname    = $lname = $upload = $desc = $tag = $copyright = $privateAccess = $access = "";
        }//if
    }//if
    

    if ($fname != "" && $lname != "" && $uploaded == true && $desc != "" && $tag != "" && $copyright != "") {
        //store each value in array $output
        $output = array(
            'firstname' => $fname,
            'lastname' => $lname,
            'access' => $access,
            'tags' => $tag,
            'description' => $desc,
            'permission' => $copyright,
            'imageFile' => $UID . '.' . $imageFileType,
            'UID' => $UID,
            'approved' => "false"
        );
        
        echo "<h2>PHP is Fun!</h2>";
        //increment UID if upload is successful
        $UID = str_pad(file_get_contents('identifier.txt') + 1, 4, '0', STR_PAD_LEFT);
        file_put_contents('identifier.txt', $UID);
        
        // read json file into array of strings
        $file      = "galleryinfo.json";
        $filearray = file($file);
        
        // create one string from the file
        $jsonstring = "";
        foreach ($filearray as $line) {
            $jsonstring .= $line;
        }//foreach
        
        //decode the string from json to PHP array
        $phparray = json_decode($jsonstring, true);
        
        // add form submission to data (this does NOT remove submit button)
        $phparray[] = $output;
        
        // encode the php array to formatted json 
        $jsoncode = json_encode($phparray, JSON_PRETTY_PRINT);
        
        // write the json to the file
        file_put_contents($file, $jsoncode);
        
        $success = "Your image was submitted. Once a moderator approves it, you will be able to view it."; 
        
        //reset form after successful upload
        $fnameErr = $lnameErr = $uploadErr = $descErr = $tagErr = $copyrightErr = "";
        $fname    = $lname = $upload = $desc = $tag = $copyright = $privateAccess = $access = "";

    }//if
    
    include "form.inc";
    
} else {
    include "gallery.inc";
}//else

//eliminate white space and special char
function test_input($data)
{
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}// test_input


include "footer.inc";

?>