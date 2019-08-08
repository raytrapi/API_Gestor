<?php
namespace src\core;

class Mensaje
{

    private static $mensaje = array();

    /**
     * Carga la variable Mensaje
     *
     * @param mixed $valor
     *            Valor a cargar en la variable mensaje
     * @param mixed $valorOpcional
     *            Valor adicional
     * @param array $opciones
     *            Opciones de carga
     */
    static public function setMensaje($valor, $valorOpcional = NULL, $opciones = array())
    {
        self::$mensaje = $valor;
    }

    /**
     * Generas una salida con el formato solicitado
     *
     * @param mixed $valores  Valores a convertir
     * @return string
     */
    static public function mostrar($valores = [])
    {
        return self::conversion($valores);
    }

    static private function conversion($valores, $nivel = 1, $opciones=[]){
        $resultado = "";
        if(key_exists("encapsular", $opciones)){
            $encapsular=$opciones["encapsular"];
        }else{
            $encapsular=true;
        }

        switch (Peticion::$formatoRespuesta) {
            case Peticion::JSON:
                if ((Peticion::$encapsulado && $nivel==1) || ($nivel>1 && $encapsular)) {
                    $resultado = "{";
                }

                if (is_array($valores)) {
                    if(key_exists(0, $valores)){
                        $i=0;
                        $resultado.="[";
                        foreach ($valores as $v) {
                            $resultado .= ($i>0?",":""). self::conversion($v,$nivel+1);
                            $i++;
                        }
                        $resultado.="]";
                    }else{
                        $i=0;
                        foreach ($valores as $k => $v) {
                            if(!is_array($v)){
                              $resultado .= ($i>0?",":"")."\"".$k."\":";
                              if(is_numeric($v)){
                                  $resultado.= $v;
                              }else {
                                  $resultado.= "\"".$v."\"";
                              }
                            }else{
                                
                                $resultado .= ($i>0?",":"")."\"".$k."\":" . self::conversion($v,$nivel+1, ["encapsular"=>(!key_exists(0,$v))]);
                            }
                            $i++;
                        }
                    }
                } else {
                    $resultado .= "\"objeto\":" . $valores;
                }

                if ((Peticion::$encapsulado && $nivel==1) || ($nivel>1 && $encapsular)) {
                    $resultado .= "}";
                }
                break;
        }

        return $resultado;
    }
}
