(deftemplate aerodromo
    (slot identificador (type INTEGER))
    (slot ciudad)
    (slot estado (allowed-values ON OFF))
    (slot visibilidad (type INTEGER))
    (slot velocidadViento (type INTEGER))
)

(deftemplate aeronave
    (slot company)
    (slot idAerodromoOrigen (type INTEGER))
    (slot idAerodromoDestino (type INTEGER))
    (slot velocidad)
    (slot Peticion (allowed-values Ninguna Despegue Aterrizaje Emergencia Interceptación Información Rumbo))
    (slot estado (allowed-values enTierra Ascenso Crucero Descenso) (default enTierra))
)

(deftemplate piloto
    (slot idAeronave (type INTEGER))
    (slot Estado (allowed-values OK SOS Ejecutando Stand-by) (default Stand-by))
)

(deftemplate vuelo
    (slot aerodromoOrigen (type INTEGER))
    (slot aerodromoDestino (type INTEGER))
    (slot velocidadDespegue (type INTEGER) (default 240))
    (slot velocidadCrucero (type INTEGER) (default 700))
)

(defrule Despegar
    ?aeronave <- (aeronave (Estado enTierra) (Peticion Despegue) (idAerodromoOrigen ?id1) (idAerodromoDestino ?id2))
    ?aerodromo <- (aerodromo (identificador ?id1) (visibilidad ?vision) (velocidadViento ?viento))
    ?piloto <- (piloto (Estado OK))
    (vuelo (velocidadDespegue ?despegue))
    (test > ?vision 5)
    (test > ?viento < 75)
    =>
    (modify ?piloto (Estado Ejecutando))
    (modify ?aeronave (Estado Ascenso) (velocidad ?despegue) (Peticion Ninguna))
)

(defrule Excepcion
    ()
)