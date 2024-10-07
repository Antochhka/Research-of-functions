document.getElementById('form').addEventListener('submit', function(event) {
  event.preventDefault(); 
  
  const functionInputValue = document.getElementById('functionInput').value; 

 
  const formData = new FormData();
  formData.append('function', functionInputValue);

  // Отправляем данные через POST-запрос
  fetch('http://localhost:8080', { 
    method: 'POST',
    body: formData
  })
  .then(response => {
    if (!response.ok) {
      throw new Error('Network response was not ok');
    }
    return response.json();
  })
  .then(data => {
    console.log('Success:', data); 
  })
  .catch(error => {
    console.error('Error:', error); 
  });
});


function plotGraph(event) {
    event.preventDefault(); 

    const input = document.getElementById('functionInput').value;
    const xValues = [];
    const yValues = [];

    // Генерация значений x и y
    for (let x = -10; x <= 10; x += 0.1) {
        xValues.push(x);
        try {
            // вычисление значения функции
            yValues.push(eval(input));
        } catch (error) {
            alert('Ошибка в функции. Проверьте правильность написания.');
            return;
        }
    }

    const trace = {
        x: xValues,
        y: yValues,
        type: 'scatter'
    };

    const data = [trace];

    Plotly.newPlot('plot', data);
}



document.querySelector('button').addEventListener('click', function (e) {
    var div = document.querySelector('div')
    div.style.display = div.style.display = 'block'
})