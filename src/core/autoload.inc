<?php
spl_autoload_register(function ($clase) {
    $partes = explode("\\", $clase);
    $partes[count($partes) - 1] = strtolower($partes[count($partes) - 1]) . ".inc";
    $clase = implode("\\", $partes);
    require_once $clase;
    //echo "Intento cargar la clase [" . $clase . "]<br/>";
});