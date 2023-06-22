(deftemplate aeronave
  (slot identificador)
  (slot compania)
  (slot aerodromo-origen)
  (slot aerodromo-destino)
  (slot velocidad-actual)
  (slot peticion (default Ninguna) (allowed-values Ninguna Despegue Aterrizaje Emergencia Intecerptacion Informacion Rumbo))
  (slot estado (default enTierra) (allowed-values enTierra Ascenso Crucero Descenso))
)

(deftemplate aerodromo
  (slot identificador)
  (slot ciudad)
  (slot estado-radar (allowed-values ON OFF))
  (slot visibilidad)
  (slot velocidad-viento)
)

(deftemplate piloto
  (slot identificador)
  (slot aeronave-asignada)
  (slot estado (default Stand-by) (allowed-values OK SOS Ejecutando Stand-by))
)

(deftemplate vuelo
  (slot identificador-origen)
  (slot identificador-destino)
  (slot distancia)
  (slot velocidad-despegue (default 240))
  (slot velocidad-crucero (default 700))
)

(defrule Despegue
  ?a <- (aeronave (identificador ?id) (aerodromo-origen ?origen)
          (estado enTierra) (peticion Despegue))
  ?p <- (piloto (aeronave-asignada ?id) (estado OK))
  ?d <- (aerodromo (identificador ?origen) (estado-radar ON) (visibilidad ?v)
         (velocidad-viento ?w))
  (test (> ?v 5))
  (test (< ?w 75))
  =>
  (printout t "El vuelo" ?id "va a realizar la acción despegue desde el aeródromo" ?origen crlf)
  (modify ?a (estado Ascenso) (peticion Ninguna))
  (modify ?p (estado Ejecutando))
)

(defrule Excepcion
  ?a <- (aeronave (identificador ?id) (peticion Despegue) (estado enTierra))
  (not (piloto (aeronave-asignada ?id)))
  =>
  (printout t "El vuelo" ?id "ha declarado una emergencia debido a la ausencia de piloto." crlf)
  (modify ?a (peticion Emergencia))
)

(deffunction calcular-horas (?dist ?speed)
  (return (div ?dist ?speed))
)

(deffunction calcular-minutos (?dist ?speed)
  (return (mod ?dist ?speed))
)

(defrule Crucero
  ?a <- (aeronave (identificador ?id) (estado Ascenso))
  ?p <- (piloto (aeronave-asignada ?id) (estado Ejecutando))
  ?v <- (vuelo (identificador ?id) (distancia ?dist) (velocidad-crucero ?cruiser-speed))
  =>
  (printout t "El vuelo" ?id "ha alcanzado la velocidad de crucero." crlf)
  (modify ?a (estado Crucero) (velocidad-actual ?cruiser-speed))
  (modify ?p (estado Stand-by))
  (printout t "El tiempo estimado de vuelo es" (call calcular-horas ?dist ?cruiser-speed) "horas y"
             (call calcular-minutos ?dist ?cruiser-speed) "minutos." crlf)
)
