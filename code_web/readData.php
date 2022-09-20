<?php
    header('Content-Type: application/json');

    // dang nhap vao database
    $server = "localhost";
    $user = "hunhun"; 
    $pass = "22112001";
    $dbname = "loadcell";

    // Connect to database
    $conn = mysqli_connect($server,$user,$pass,$dbname);

    // Check connection
    if($conn === false){
        die("ERROR: Could not connect. " . mysqli_connect_error());
    }

    // Doc gia tri ax ay az tu database
    $sql = "select * from value_box where stt>(select max(stt) from value_box)-1";
    $result = mysqli_query($conn,$sql);

    $data = array();
    foreach ($result as $row){
        $data[] = $row;
    }

    mysqli_close($conn);
    echo json_encode($data);

?>