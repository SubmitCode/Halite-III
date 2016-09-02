<!DOCTYPE html>
<html lang="en">

<head>
	<?php include 'includes/header.php'; ?>

	<title>Forums Login</title>

	<link href="lib/bootstrap.min.css" rel="stylesheet">
	<link href="style/general.css" rel="stylesheet">
</head>

<body>
	<div class="container">
		<div id="messageBox"></div>
		<div class="pageContent">
			<div class="row">
				<div class="col-sm-12">
					<div class="panel panel-primary">
						<div class="panel-heading">
							<h3 class="panel-title">Halite Forums Login</h3>
						</div>
						<div style="padding: 16px; padding-bottom: 24px;">
							<?php include 'includes/login_form.php'; ?>
						</div>
					</div>
					<span>To access the forums, you must be logged into your Halite account or must log in above. <b>Don't have an account?</b> <a href="index.php">Register on our homepage</a>.</span>
				</div>
			</div>
		</div>
	</div>

	<script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.4/jquery.min.js"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js"></script>
	<script src="script/backend.js"></script>
	<script src="script/general.js"></script>
	<script src="script/forums_auth.js"></script>
</body>

</html>
