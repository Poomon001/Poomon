<!--to display the lightbox-->
<div id='lightbox' class='hidden'></div>
	<div id='positionBigImage'>
		<div id='boundaryBigImage' class='hidden'>
		
			<a  href='javascript:displayLightBox("none", -1)'> 
				<img id= 'x' src='images/close.png' alt='close'>
			</a>
			
			<div id="imageButton">
				<img id='bigImage' src='images/close.png' alt='close'>
				<a href='javascript:displayLightBox("none", -1)' id='previous'><img src="images/left.png" id="left" alt="left arrow"></a>
				<a href='javascript:displayLightBox("none", -1)' id='next'><img src="images/right.png" id="right" alt="right arrow"></a>
				<a href="" id='download' download='null'><img src="images/download.png" id="downloadbutton" alt="download"></a>
			</div>
			<div id='name' class='lightboxText'></div>
			<div id='photoDesc' class='lightboxText'></div>
			<?php
			if ($isEditor) {
				echo"<form id='posotionForm' method='post'>
					<h4>First name</h4><br><input type='text' name='fnameSingle' id='fnameSingle'><br>
					<h4>Last name</h4><br><input type='text' name='lnameSingle' id='lnameSingle'><br>
					<h4>Description</h4><br><input type='text' name='descSingle' id='descSingle'><br>
					<h4>Tags<br></h4><input type='text' name='tagsSingle' id='tagsSingle'>
					<br>
					<button type='submit' id='editSingle' name='editSingle' value='true'>Edit</button>
				</form>";
			}//if
			?>
		</div>
	</div>