import { initializeApp } from "https://www.gstatic.com/firebasejs/9.13.0/firebase-app.js";
import { getDatabase, ref , onValue } from "https://www.gstatic.com/firebasejs/9.13.0/firebase-database.js";


const firebaseConfig = {
    apiKey: "AIzaSyCPYHYU1Ld6mCjIkC1a9ANv6ka-72VcpvM",
    authDomain: "bastonguia-d271d.firebaseapp.com",
    databaseURL: "https://bastonguia-d271d-default-rtdb.firebaseio.com",
    projectId: "bastonguia-d271d",
    storageBucket: "bastonguia-d271d.appspot.com",
    messagingSenderId: "443829963777",
    appId: "1:443829963777:web:ea2bd5f7ffd22354ea8400"
};

const app = initializeApp(firebaseConfig);
const db = getDatabase(app);

const openModal = document.getElementById("start");
const elementModal = document.getElementById("windowMod");
const closeModal = document.getElementById("closeModal");
const showTable = document.getElementById("sensors");
const SensorValue = document.getElementById("sonicValue");
const ldrValue = document.getElementById("ldrValue");

const lecturaSensor = ref(db, 'StickGuide');

onValue(lecturaSensor, (snapshot) =>{
  const data = snapshot.val();
  const dato = Object.keys(data);

  var k =[];
  var ys =dato.slice(0,1);

  k.push(ys);
  k.forEach(item =>{
     
    var valSen = data[item];
    console.log(valSen);

    SensorValue.innerHTML = valSen;

  });
  
  var l;
  var valLuz;

  for(var i = 0; i < dato.length; i ++){

    l = dato[i];
    valLuz = data[l];
  }

  console.log(valLuz);
  ldrValue.innerHTML = valLuz;


});




const pupUpMod = () => {
  elementModal.classList.toggle("is-active");
};

const closeWind = () => {
  elementModal.classList.toggle("is-active");
  showTable.style.display = "flex";
};



openModal.addEventListener("click", pupUpMod);
closeModal.addEventListener("click", closeWind);
