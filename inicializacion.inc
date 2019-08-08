<?php
if (! defined("__DIR__")) {
    define("__DIR__", dirname(__FILE__));
}

date_default_timezone_set("Europe/Madrid");

require_once 'src\core\autoload.inc';
require_once 'conf\global.inc';
require_once 'src/seguridad/controlErrores.inc';