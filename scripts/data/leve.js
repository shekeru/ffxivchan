 array = {}; $("td.valign.details").each(function() {
   let el = $(this);
   let leve = el.find("h4>a"), item = el.find("p>a");
   let leveId = leve.attr('href').split("/");
   let itemId = item.attr('href').split("/");
   let qty = el.find("p>span").text().split(" ");
   array[Number(leveId.pop())] = {
    leveName: leve.text(),
    quantity: Number(qty.pop() || "1"),
    repeated: Boolean(el.find("p>i").length),
    itemId: Number(itemId.pop()),
    itemName: item.text(),
   };
}); console.log(JSON.stringify(array));
