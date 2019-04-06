## Map
* Tile Size 160 * 160
* Map Size 12 * 6

### MapData
{
	width: int
	heigt: int
	fieds: [
		{
	]
}

## Menschen

### Eigenschaften
* Job -> Schicht/Geld
* PolitikMeter (mag dich -- neutral -- mag Gegener)
* Zufriedenheit -> beeinflusst wahl (contra aktueller Mehrheit)
* Aktivitätsmotivation
* Gesundheit/Fitness

## Aktionen

### Kosten
* Geld

### Arten

**Rede**
* local
* pro dich

**Cherety**
* glopal
* polarisation

**Polizei**
* löst demon
* verringert zuneigung
* verringert gesundheit
* verringert Aktivitätsmotivation bei geringer unzufiriedener sonst erhöt

**Flugblätter**
* contra Gegener

## Hobbys

### Parameter
* steuern
* eintsellung

### Arten
* Theater
   * reich
   * gebildet
* Tennis
   * mittel + reich
   * sportlich
* Skaten
   * arm
* Acard
   * mittel
   
## Events

**Aufbau**
* Bendingung
* Phänotyp
* Reaktionen, Kosten:Effekte

### Demo

1.
   * Unzufriedenheit > 
   * Aktivitätsmotivation > (je mehr leute und umso unzufiriedener -> weniger)
2.
   * leute Protestieren
3.
   * Nichts : Passanten werden Angesteckt
   * Polizei :
      * Demo leute Aktivitätsmotivation sinkt
	  * Unzufriedenheit steigt, mögen dich weniger
	  * passanten mögen dich nicht
   * Rede:
      * Für dich: steigt zuneigung
	  * gegen dich: Aktivitätsmotivation sinkt, (zuneigung steigt?)
   * Flugblätter:
      * Für dich: steigt zuneigung

### Randele
1. Aktivitätsmotivation >
2. besetztes Feld
3. Nichts: Unzufriedenheit der betroffenen, 
   Polizei:
      * zuneigung der Betroffenen steigt
	  * Aktivitätsmotivation der Randalierer sinkt schwach
   Cherety:
      * Aktivitätsmotivation sinkt stark (der randalierer)
	  * zuneigung aller steigt stark

### Feiern/Umzung
1. zufriedenheit >, zufall
2. 
   * Feier Geslschaft
3.
   * Nichts: zufriedenheit steigt (schwach)
   * Rede: zuneigung steigt
   * Polizei: zufriedenheit sinkt (stark)
   * Immer: einahmen sinken(schwach)

### Stromausfall
1. zufall
2.
   * facillity kaputt
3.
   * Aufschieben:
      * Unzufriedenheit der Betroffenen
   * Reparieren, Geld:
      * zuneigung steigt

### Koruption Geld klau
1. einfluss >
2. Meldung
3.
   * Nichts:
      * Geld verlust
      * vllt zuneigung und zufriedenheit sinkt
   * Geheimdienst, Einfluss >:
      * vllt Geld zurück
	  * vll zuneigung und zufriedenheit singt (stark)
   * Öffentlich
      * zufriedenheit singt
	  
### Koruption Verrat
1. einfluss >, zuneigung < oder unzufiriedener >
2. Meldung
3.
   * Nichts:
      * Einfluss sinkt
   * Inhaftieren:
      * zuneigung sinkt & zufriedenheit
   * Bestechen, Geld:
   * Geheimdienst, Einfluss >:
      * vllt unzufiriedener & zuneigung sinkt stark
	  
### Koruption beim gegner
1. einfluss <
2. Meldung
3. 
    * untersützen, Geld:
	   * einfluss steigt
	   * vllt zuneigung sinkt
	* Nichts:
	   * vllt zufriedenheit sinkt
	* Anprangen:
	   * zuneigung steigt
	   