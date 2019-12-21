<!---form that allow the user to upload their baby photos with their name and details -->
<form class = "myForm"  method="post" enctype="multipart/form-data">
   <p id="title"> Image Gallery </p>
   <div id="firstName">First Name </div>
   <input type = "text" name = "fname" value="<?php echo $fname ?>">
   <span class="error"><?php echo $fnameErr;?></span>
   <div id="lastName">Last Name</div>
   <input type = "text" name = "lname" value= "<?php echo $lname ?>" >
   <span class="error"><?php echo $lnameErr;?></span>
   <div id="photo">Photo</div>
   <input type="file" name ="upload" id="fileToUpload">
   <span class="error"><?php echo $uploadErr;?></span>
   <div id="tags">Tags</div>
   <textarea type="tags" name="tag" > <?php echo $tag ?></textarea>
   <span class="error"><?php echo $tagErr;?></span>
   <div id="description">Description</div>
   <textarea name="desc"> <?php echo $desc ?></textarea>
   <span class="error"><?php echo $descErr;?></span>
   <br>
   <input type="checkbox" name="copyright" value="copyright"<?php if (isset($copyright) && $copyright=="copyright") echo "checked";?> > I retain the copyright to this image and release it for use on this site<br>
   <span class="error"><?php echo $copyrightErr;?></span>
   <br>
   <div id="public">
      <input class = "buttons" type="radio" name="access" value="public"<?php if (isset($access) && $access =="public") echo "checked";?>> Public <br>
      <input class = "buttons"type="radio" name="access" value="private"<?php if (isset($access) && $access =="private") echo "checked";?> > Private<br>
   </div>
   <br>
   <button id="submit" type="submit"value="submit" name="submit">Submit</button> 
   <button id="reset" type="reset" value="reset"name="reset" >Reset</button>
   <a id="image" href="index.php" class='glyphicon glyphicon-picture' style="font-size:48px; position: relative; top: 20px; left: 20px;"></a>
</form>