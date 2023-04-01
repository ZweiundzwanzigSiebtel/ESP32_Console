#ifndef HTML_PAGE_H
#define HTML_PAGE_H

#include <Arduino.h>

const static String html_site = R"(<!DOCTYPE html>
<html>
  <head>
    <meta charset='UTF-8'>
    <title>Kreis-Beispiel</title>
    <style>
      #kreis-container {
        position: relative;
        width: 300px;
        height: 300px;
        border: 1px solid black;
        border-radius: 50%;
      }
      #innerer-kreis {
        position: absolute;
        width: 50px;
        height: 50px;
        background-color: red;
        border-radius: 50%;
        left: 50%;
        top: 50%;
        transform: translate(-50%, -50%);
        cursor: move;
      }
    </style>
  </head>
  <body>
    <div id='kreis-container'>
      <div id='innerer-kreis'></div>
    </div>

    <script>
      // Funktion, um die Position des inneren Kreises zu aktualisieren und an den Server zu senden
      function sendePosition(x, y) {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', '/controller1?x=' + x + '&y=' + y, true);
        xhr.send();
      }

      // Elemente aus dem DOM holen
      var innererKreis = document.getElementById('innerer-kreis');
      var kreisContainer = document.getElementById('kreis-container');

      // Funktion, um die Position des inneren Kreises zu aktualisieren
      function aktualisierePosition(event) {
        // Neue Position berechnen
        var x = event.clientX - kreisContainer.getBoundingClientRect().left;
        var y = event.clientY - kreisContainer.getBoundingClientRect().top;

        // Begrenzen der Position innerhalb des großen Kreises
        var radius = kreisContainer.offsetWidth / 2;
        var dist = Math.sqrt((x - radius) ** 2 + (y - radius) ** 2);
        if (dist > radius) {
          var angle = Math.atan2(y - radius, x - radius);
          x = Math.round(radius + Math.cos(angle) * radius);
          y = Math.round(radius + Math.sin(angle) * radius);
        }

        // Position des inneren Kreises aktualisieren
        innererKreis.style.left = x + 'px';
        innererKreis.style.top = y + 'px';

        // Position an den Server senden
        sendePosition(x, y);
      }

      // Event-Listener hinzufügen
      innererKreis.addEventListener('mousedown', function(event) {
        kreisContainer.addEventListener('mousemove', aktualisierePosition);
      });

      kreisContainer.addEventListener('mouseup', function(event) {
        kreisContainer.removeEventListener('mousemove', aktualisierePosition);
      });
    </script>
  </body>
</html>
)";
/****
R"(<head>
	<meta charset="UTF-8">
	<title>Steering</title>
	<style>
		body {
			font-family: Arial, Helvetica, sans-serif;
		}

		button {
			font-size: 20px;
			padding: 10px;
			margin: 10px;
		}
	</style>
</head>

<body>
h1>Steering</h1>
	<button onclick=' sendDown()'>Steer Down</button>
	<button onclick='sendUp()'>Steer Up</button>
	<button onclick='sendLeft()'>Steer Left</button>
	<button onclick='sendRight()'>Steer Right</button>
	<script>
		function sendUp() {
			var xhr = new XMLHttpRequest();
			xhr.open('GET', '/up', true); xhr.send();
		} function sendDown() {
			var xhr = new XMLHttpRequest();
			xhr.open('GET', '/down', true); xhr.send();
		} function sendLeft() {
			var xhr = new XMLHttpRequest();
			xhr.open('GET', '/left', true); xhr.send();
		} function sendRight() {
			var xhr = new XMLHttpRequest();
			xhr.open('GET', '/right', true); xhr.send();
		} </script>
</body>

</html>)";
*/
#endif