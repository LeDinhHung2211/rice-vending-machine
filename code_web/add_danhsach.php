<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>MPU_6050</title>
    <link rel="stylesheet" href="style_danhsach.css">
    <!-- <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">  -->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css">
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.1/mqttws31.js" type="text/javascript"></script>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/3.2.0/chart.min.js"> </script>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://kit.fontawesome.com/b7f0c56a4e.js" crossorigin="anonymous"></script>
    <script src="script.js"></script>
</head>
<body> 
    

    <div id="header">
        <!-- Begin nav -->
        <ul id="nav">
            <li><a href="loadcell.html"  onclick="memberFunction()"><i class="fa-solid fa-user"></i> Member </a></li>
            <li><a href="loadcell.html"  onclick="configFunction()"><i class="fa-solid fa-sliders"></i> Config </a></li>
            <li><a href="loadcell.html"  onclick="displayFunction()"><i class="fa-solid fa-chart-line"> </i>Analysis</a></li>
            <li><a  onclick="chartFunction()"><i class="fa-solid fa-list"></i>Danhsach</a></li>
            <li><a href="loadcell.html" onclick="aboutFunction()"><i class="fa-solid fa-circle-info"></i> About </a></li>
        </ul>


    </div>
    <br>
    <br>
    <br>
        <!-- end nav -->


    <!-- DanhSach -->

     <!-- Danhsach -->
     <div class="list">
     
        <h1> DANH SACH</h1>
        
        <table class="table table-striped table-hover"">
                <tr>
                    <th>STT</th>
                    <th>Color</th>
                    <th>khoi luong</th>
                    <th>Date</th>
                    
                </tr>
            <?php
                $server = "localhost";
                $user = "hunhun"; 
                $pass = "22112001";
                $dbname = "loadcell";
            
                $conn = mysqli_connect($server,$user,$pass,$dbname);
                $sql = "select stt, color, khoiluong, date from danhsach";
                $result = mysqli_query($conn,$sql);
                while ($row = mysqli_fetch_array($result)){
            ?>
                <tr>
                    <td><?php echo $row['stt']; ?></td>
                    <td><?php echo $row['color']; ?></td>
                    <td><?php echo $row['khoiluong']; ?></td>
                    <td><?php echo $row['date']; ?></td>
                </tr>
                <?php } ?> 
            

           
        </table>
    <script>
        var rgb = [];
        $(document).ready(function(){
            //gui request xuong db de lay data
            $.post('readData.php',function(data){
                rgb[0] = data[0].green;
                rgb[1] = data[0].white;
                rgb[2] = data[0].others;
                // console.log(data1);
            });
        });
    </script>

    <form hidden id="ax_test">
        <input name="bien_color" id="bien_color_id" >
        <input name="bien_kl" id="bien_kl_id" >
    </form>

    <form hidden id="value_chart">
        <input name="green_chart" id="green_chart_id" >
        <input name="white_chart" id="white_chart_id" >
        <input name="others_chart" id="others_chart_id" >
    </form>
</body>
</html>