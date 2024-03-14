<?php
$upload_dir = getenv("UPLOAD_DIR");
try
{
    switch ($_FILES['file']['error'])
    {
        case UPLOAD_ERR_OK:
            break;
        case UPLOAD_ERR_NO_FILE:
            throw new RuntimeException('No file sent.');
        case UPLOAD_ERR_INI_SIZE:
        case UPLOAD_ERR_FORM_SIZE:
            throw new RuntimeException('Exceeded filesize limit.');
        default:
            throw new RuntimeException('Unknown errors.');
    }
    if ($_FILES['file']['size'] > 1000000) {
        throw new RuntimeException('Exceeded filesize limit.');
    }
    $filename = sprintf('%s/%s', $upload_dir, 
    $_FILES['file']['name']);
    if (!move_uploaded_file(
        $_FILES['file']['tmp_name'],
        $filename)) {
        throw new RuntimeException('Failed to move uploaded file.');
    }
    echo "<html><head></head><body>";
    echo "<h1>FILE UPLOADED SUCCESFULLY</h1>";
    echo "</body>";
}
catch (RuntimeException $e)
{
    echo "<html><head></head><body>";
    echo "<h1>Error</h1>";
    echo "<h2>";
    echo "$e->getMessage()";
    echo "</h2>";
    echo "</body>";
}
?>