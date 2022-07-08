const out = document.querySelector('.screen p'); 
const pi = Math.PI; 
const e = Math.E;
const occurate = 12;
const flex_container = document.querySelector('.flex');

let a = '';
let b = '';
let sign = '';
let finish = false;
let mool = Math.pow(10, occurate);

const num = ['0','1','2','3','4','5','6','7','8','9','.'];
const action = ['-','+','X','/','%','+/-','<=','ln','exp','|x|','π','e',
                'x2','xy'];

let resize = function (event) {
    let height_flex = document.querySelector('.flex').offsetHeight;
   let height_body = document.body.offsetHeight;

   if (height_body < height_flex) {
        document.body.style.height = height_flex + "px";
   }
}
window.onresize = resize;
window.onload = resize;

document.querySelector('.flex').onclick = (event) => {
    if(!event.target.classList.contains('knopochka')) return;

    if (event.target.classList.contains('AC')) {
        a = '';
        b = '';
        sign = '';
        finish = false;
        out.textContent = '';
    };

    if (event.target.classList.contains('clear')) {  
        if (b === '' && sign === '') {
            a = a.substring(0, a.length - 1);
            console.log(a, b, sign);
            out.textContent = a;
        } else if (a !== '' && b !== '' ) {
            b = b.substring(0, b.length - 1);
            out.textContent = b;
        } else {
            b = b.substring(0, b.length - 1);
            out.textContent = b;
            console.log(a, b, sign);
        };
        return;    
    }; 

    if (event.target.classList.contains('sqr')) {
        a = Math.pow(a, 2);
        out.textContent = a;
        return;
    };

    if (event.target.classList.contains('ln')) {
        a = Math.log(a);
        out.textContent = a;
        return;
    };

    if (event.target.classList.contains('exp')) {
        a = Math.exp(a);
        out.textContent = a;
        return;
    };

    if (event.target.classList.contains('abs')) {
        a = Math.abs(a);
        out.textContent = a;
        return;
    };

    if (event.target.classList.contains('sqrt')) {
        a = Math.sqrt(a);
        out.textContent = a;
        return;
    };
    if (event.target.classList.contains('change')) {
        if (b === '' && sign === '') {
            a = (-a);
            out.textContent = a;
        } else if (a !== '' && b !== '' && finish) {
            b = (-b);
            finish = false;
            out.textContent = b;
        } else {
            b = (-b);
            out.textContent = b;
        } return;
    };

    if (event.target.classList.contains('pi')) {
        if (b === '' && sign === '') {
            a = pi;
            console.log(a, b, sign);
            out.textContent = 'π';
        } else if (a !== '' && b !== '' ) {
            b = pi;
            out.textContent = 'π';
        } else {
            b = pi;
            out.textContent = 'π';
        }
        return;
    };

    if (event.target.classList.contains('e')) {
        if (b === '' && sign === '') {
            a = e;
            out.textContent = 'e';
        } else if (a !== '' && b !== '' ) {
            b = e;
            out.textContent = 'e';
        } else {
            b = e;
            out.textContent = 'e';
        }
        return;
    };

    const key = event.target.textContent;
    if (num.includes(key)) {
        if (b === '' && sign === '') {
            a += key;
            out.textContent = a;
        } else if (a !== '' && b !== '' && finish) {
            b = key;
            finish = false;
            out.textContent = b;
        } else {
            b += key;
            out.textContent = b;
        } return;        
    };   

    if (action.includes(key)) {
        sign = key;
        out.textContent = sign;
        return;
    };

    if (key === '=') {
        switch(sign) {
            case "+" :
                a = (+a) + (+b);
                break;
            case "-" :
                a = (+a) - (+b);
                break;           
            case "X" :
                a = (+a) * (+b);
                break;
             case "/" :
                if (b === '0') {
                    out.textContent = 'па жопе палучишь';
                    return;
                }
                a = (+a) / (+b);
                break;
            case "%" :
                if (b === '') {
                    a = (+a) / 100
                } else {
                    a = (a * b) / 100;  
                };
                break; 
            case 'xy':
                a = Math.pow(a, b);
                break;   
            }
        finish = true;
        out.textContent = Math.round(mool * a) / mool;
    }
};
