const drawer = document.getElementById("drawer");
let mouseOver = false; //!To fix any scrolling while hovering over the menu or drawer
document.addEventListener("DOMContentLoaded", function () {
  const navIcon = document.getElementById("menu-icon");
  navIcon.addEventListener("mouseenter", function () {
    drawer.classList.add("active");
    mouseOver = true;
  });

  navIcon.addEventListener("mouseleave", function () {
    drawer.classList.remove("active");
    mouseOver = false;
  });

  drawer.addEventListener("mouseenter", function () {
    drawer.classList.add("active");
    mouseOver = true;
  });

  drawer.addEventListener("mouseleave", function () {
    drawer.classList.remove("active");
    mouseOver = false;
  });
});

//? Making the navbar disappear on scrolling
const navBar = document.getElementById("navBar");
var prevScrollPos = window.scrollY;
window.onscroll = () => {
  var currentScrollPos = window.scrollY;
  if (prevScrollPos > currentScrollPos || mouseOver === true) {
    navBar.style.top = "0";
  } else {
    navBar.style.top = "-50px";
  }
  prevScrollPos = currentScrollPos;
};

function initMap() {
  var map = new google.maps.Map(document.getElementById("map"), {
    center: { lat: 0, lng: 0 },
    zoom: 3,
  });

  var marker = new google.maps.Marker({
    position: { lat: 0, lng: 0 },
    map: map,
    title: "iam here",
  });

  function fetchRobotLocation() {
    fetch("/api/robot-location")
      .then((response) => response.json())
      .then((data) => {
        marker.setPosition({ lat: data.latitude, lng: data.longitude });
        map.setCenter({ lat: data.latitude, lng: data.longitude });
      })
      .catch((error) => console.error("Error fetching robot location:", error));
  }
  // to update robot location every 5 seconds
  setInterval(fetchRobotLocation, 5000);
}
function fetchSensorData() {
  fetch("/api/sensor-data")
    .then((response) => response.json())
    .then((data) => {
      document.getElementById("temperature").textContent = data.temperature
        ? data.temperature + " °C"
        : "N/A";
      document.getElementById("pressure").textContent = data.pressure
        ? data.pressure + " hPa"
        : "N/A";
      document.getElementById("humidity").textContent = data.humidity
        ? data.humidity + " %"
        : "N/A";
      document.getElementById("doppler").textContent = data.doppler
        ? data.doppler + " m/s"
        : "N/A";
    })
    .catch((error) => {
      console.error("Error fetching sensor data:", error);
    });
}
setInterval(fetchSensorData, 5000);
fetchSensorData();
