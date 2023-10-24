const products = {
    "p1": {
        "name": "Картофель",
        "price": 150,
        "subproducts": [ ],
        "subproperty": { }
    },
    "p2": {
        "name": "Помидоры",
        "price": 120,
        "subproducts": [
            {
                "name": "Орлиное сердце",
                "price": 100
            },
            {
                "name": "Кенигсберг",
                "price": 120
            },
            {
                "name": "Абаканский розовый",
                "price": 150
            },
            {
                "name": "Чио-чио-сан",
                "price": 180
            }
        ],
        "subproperty": { }
    },
    "p3": {
        "name": "Огурцы",
        "price": 100,
        "subproducts": [ ],
        "subproperty": {
            "name": "Отборные",
            "multiplier": 0.9
        }
    },
    "p4": {
        "name": "Морковь",
        "price": 180,
        "subproducts": [
            {
                "name": "Самсон",
                "price": 200
            },
            {
                "name": "Анастасия",
                "price": 220
            },
            {
                "name": "Наярит",
                "price": 200
            },
        ],
        "subproperty": {
            "name": "Свежие",
            "multiplier": 1.1
        }
    },
    "p5": {
        "name": "Грибы",
        "price": 150,
        "subproducts": [ ],
        "subproperty": { }
    },
}

function loadSubproductsOptions(subproducts, subproductsSelectEl) {
    if (subproducts.length > 0) {
        subproductsSelectEl.removeAttribute("disabled");
    }
    else {
        subproductsSelectEl.setAttribute("disabled", "");
    }
    // Добавление сортов
    subproducts.forEach(subproduct => {
        let option = document.createElement("option");
        let name = document.createTextNode(subproduct["name"]);
        option.setAttribute("value", subproduct["price"]);
        option.appendChild(name);
        subproductsSelectEl.appendChild(option);
    });
}

function changeProductType() {
    let subproductsEl = document.getElementById("subproducts");
    let typeSelectEls = document.getElementsByName("product-type");
    let selectedType;

    // Ищем выбранный продукт
    typeSelectEls.forEach(typeSelectEl => {
        if (typeSelectEl.checked === true) {
            selectedType = typeSelectEl.value;
        }
    });
    let product = products[selectedType];

    let subproductsSelectEl = document.getElementById("subproducts-select");
    let subproductsCheckbox = document.getElementById("product-property");
    let subproductsCheckboxLabel = document.getElementById("product-property-label");
    
    subproductsSelectEl.innerHTML = " ";

    // Создание стандартного выбора в селекте
    let option = document.createElement("option");
    let name = document.createTextNode("Выберете сорт");
    option.setAttribute("value", "");
    option.appendChild(name);
    subproductsSelectEl.appendChild(option);

    // Добавление сортов в селект
    let subproducts = product["subproducts"];
    loadSubproductsOptions(subproducts, subproductsSelectEl);
    

    // Добавление доп услуги
    let subproductProperty = product["subproperty"];

    if (Object.keys(subproductProperty).length > 0) {
        subproductsCheckbox.removeAttribute("disabled");
        subproductsCheckbox.setAttribute("value", subproductProperty["multiplier"]);
        subproductsCheckboxLabel.innerText = subproductProperty["name"];
        subproductsCheckboxLabel.style.opacity = 1; 
    }
    else {
        subproductsCheckbox.setAttribute("disabled", "");
        subproductsCheckbox.setAttribute("value", "");
        subproductsCheckboxLabel.innerText = "No Property";
        subproductsCheckboxLabel.setAttribute("style", "opacity: 0");
    }
}

function calculate() {
    let quantityEl = document.getElementById("quantity-input");
    let typeSelectEls = document.getElementsByName("product-type");


    // Ищем выбранный тип
    let selectedType;
    typeSelectEls.forEach(typeSelectEl => {
        if (typeSelectEl.checked === true) {
            selectedType = typeSelectEl.value;
        }
    });
    let product = products[selectedType];


    let subproductTypeEl = document.getElementById("subproducts-select");
    let subproductPropertyEl = document.getElementById("product-property");

    // Получаем количество
    let quantity = parseInt(quantityEl.value);

    // Получаем цену товара
    let productPrice;
    if (selectedType) {
        if ((subproductTypeEl.getAttribute("disabled") === null) && !(subproductTypeEl.value === "")) {
            productPrice = subproductTypeEl.value;
        }
        else {
            productPrice = product["price"];
        }
    }

    let productMultiplier;
    if (subproductPropertyEl.getAttribute("disabled") === null && subproductPropertyEl.checked) {
        productMultiplier = subproductPropertyEl.value;
    }
    else {
        productMultiplier = 1;
    }

    let result = quantity * productPrice * productMultiplier;
    if (isNaN(result) || result < 0) {
        result = "Некорректный ввод"; 
    }

    let resultEl = document.getElementById("result");
    resultEl.innerHTML = result; 
}


document.addEventListener("DOMContentLoaded", (event) => {
    // Добавление типов продуктов
    let productsEl = document.getElementById("calc-radio-group");
    for (let i = 1; i <= Object.keys(products).length; i++) {
        let productDivEl = document.createElement("div");

        let productRadioEl = document.createElement("input");
        productRadioEl.setAttribute("type", "radio");
        productRadioEl.setAttribute("id", `radio-${i}`);
        productRadioEl.setAttribute("value", `p${i}`);
        productRadioEl.setAttribute("name", "product-type");
        productDivEl.appendChild(productRadioEl);

        let productRadioLabelEl = document.createElement("label");
        productRadioLabelEl.setAttribute("for", `radio-${i}`);
        productRadioLabelEl.innerText = products[`p${i}`]["name"];
        productDivEl.appendChild(productRadioLabelEl);

        productsEl.appendChild(productDivEl);
    }

    let selectEl = document.getElementById("calc-radio-group");
    selectEl.addEventListener("change", changeProductType);

    let buttonEl = document.getElementById("calc-button");
    buttonEl.addEventListener("click", calculate);
});