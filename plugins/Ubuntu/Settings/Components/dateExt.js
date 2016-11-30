.pragma library

function diffMonths(dateA, dateB) {
    var months;
    months = (dateB.getFullYear() - dateA.getFullYear()) * 12;
    months -= dateA.getMonth();
    months += dateB.getMonth();
    return Math.max(months, 0);
}

Date.leapYear = function(year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)
}

Date.daysInMonth = function(year, month) {
    return new Date(year, month + 1, 0).getDate();
}

Date.prototype.midnight = function() {
    var date = new Date(this)
    date.setHours(0,0,0,0);
    return date
}

Date.prototype.addDays = function(days) {
    var date = new Date(this.valueOf());
    date.setDate(date.getDate() + days);
    return date;
}

Date.prototype.addMonths = function(months) {
    var date = new Date(this)
    date.setMonth(date.getMonth() + months)
    return date
}
