<div>
  <span>obviously u need to have installed <h3><b><i> LIBPQXX </i></b></h3>on ur system.</span>
  <span><h3>POSTGRESQL</h3> is been used here</span>
  <p>install postgresql using BREW from terminal and install pgadmin4 and basically postgresql from internet</p>
</div>
<div>
  <h2><li>To compile the code (on mac)</li></h2>
  <p><h4>clang++ $(pkg-config --cflags --libs libpqxx) -std=c++17 db_connect.cpp -o db_connect</h4></p>
  <p><h5>./db_connect (to run the code...replace db_connect with your program's name)</h5></p>
</div>
<h1>Book shop Management system</h1>
<h2>Overview</h2>
<h4>This is a basic , beginner friendly project with features like CRUD operations.</h4>
<h2>Table of contents </h2>
<h3>BOOKS</h3>
<ul>
  <li>Add books</li>
  <li>Search books</li>
  <li>Search books</li>
  <li>update price and quantity of books</li>
  <li>Delete books</li>
</ul>
<h3>SUPPLIERS</h3>
<ul>
  <li>Add supplier</li>
  <li>Remove supplier</li>
  <li>Search supplier By IDsupplier</li>
</ul>
<h3>PURCHASE</h3>
<ul>
  <li>place new order</li>
  <li> View order</li>
   <h2>order status</h2>
  <li>Canceled order</li> 
  <li> Received order</li>
</ul>
<h3>EMPLOYEE</h3>
<ul>
   <li>ADD employee</li>
   <li>Remove employee</li>
   <li>Update employee details</li>
   <li>Assign as manager</li>
</ul>
<h3>MEMBERS</h3>
<h4>Refresh(); basically refreshes the date of membership of member once date of membership ends.
<ul>
  <li>Add member</li>
  <li>Remove member</li>
  <li>Refresh</li>
</ul>
<h3>SALES</h3>
<ul>
  <li>add sales</li>
  <li>Total sales </li>
</ul>
<div>
  <p>I haven't seen many resource on postgresql and C++ integerity for any Beginner friendly project like Management system so i made one just for absolute beginners.
  This basically teaches Lot more mainly about classes and postgresql and libpqxx</p>

  <p>can be improved a lot if any feel free to open issues and pull requests.</p>
</div>
