<?php
 // dang nhap vao database
    $server = "localhost";
    $user = "hunhun"; 
    $pass = "22112001";
    $dbname = "loadcell";
    
    $conn = mysqli_connect($server,$user,$pass,$dbname);
    // echo "0";
 // Check connection
    if($conn === false){
        die("ERROR: Could not connect. " . mysqli_connect_error());
    }
    $d1 =  $_POST["bien_color"];
    $d2 =  (float) $_POST["bien_kl"];
    // echo "$d2";
    $sql = "insert into danhsach (color,khoiluong) values ('$d1',$d2)";
    // echo "2";
    mysqli_query($conn,$sql);
    mysqli_close($conn);
?>