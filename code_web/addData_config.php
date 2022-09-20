<?php
 // dang nhap vao database
    $server = "localhost";
    $user = "hunhun"; 
    $pass = "22112001";
    $dbname = "config_loadcell";
    
    $conn = mysqli_connect($server,$user,$pass,$dbname);
    
    // Check connection
    if($conn === false){
        die("ERROR: Could not connect. " . mysqli_connect_error());
    }
    $d1 =  $_POST["green1"];
    $d2 =  $_POST["white1"];
    $d3 =  $_POST["others1"];

    
    $sql = "insert into value_config(green,white,others) values ($d1,$d2,$d3)";
    mysqli_query($conn,$sql);
    mysqli_close($conn);
?>