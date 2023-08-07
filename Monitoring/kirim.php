<?php
 header("Access-Control-Allow-Origin: *");
 header('Access-Control-Allow-Credentials: true');
 header('Access-Control-Max-Age: 86400');


include 'koneksi.php';


$t = $_POST['temperatur'];
$h = $_POST['kelembapan'];
$g = $_POST['gas'];


$simpan = mysqli_query($conn, "UPDATE dht SET temperatur='$t', kelembapan='$h', gas='$g' WHERE id=1 ");

if ($simpan) {
    echo json_encode(array(
        "status" => 200,
        "message" => "Data berhasil diupdate",
    ));
}else{
    echo json_encode(array(
        "status" => 404,
        "message" => "Terjadi kesalahan saat mengupdate data !",
    ));
}





?>