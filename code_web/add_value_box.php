<?php
 // dang nhap vao database
    $server = "localhost";
    $user = "hunhun"; 
    $pass = "22112001";
    $dbname = "loadcell";
    
    $conn = mysqli_connect($server,$user,$pass,$dbname);
    
    // Check connection
    if($conn === false){
        die("ERROR: Could not connect. " . mysqli_connect_error());
    }
    $d1 =  $_POST["green_chart"];
    $d2 =  $_POST["white_chart"];
    $d3 =  $_POST["others_chart"];

    
    $sql = "insert into value_box(green,white,others) values ($d1,$d2,$d3)";
    mysqli_query($conn,$sql);
    mysqli_close($conn);
?>