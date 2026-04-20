#!/bin/bash

file="library.txt"

touch $file

while true
do
    echo "------ Library Management ------"
    echo "1. Add Book"
    echo "2. Display Books"
    echo "3. Search Book"
    echo "4. Delete Book"
    echo "5. Update Book"
    echo "6. Exit"
    echo "Enter your choice:"
    read choice

    case $choice in
        1)
            echo "Enter Book ID:"
            read id
            echo "Enter Book Name:"
            read name
            echo "Enter Author Name:"
            read author
            echo "Enter Quantity:"
            read qty
            echo "$id|$name|$author|$qty" >> $file
            echo "Book Added Successfully!"
            ;;
        2)
            echo "Library Records:"
            cat $file
            ;;

        3)
            echo "Enter Book ID to search:"
            read sid
            grep "^$sid|" $file
            ;;

        4)
            echo "Enter Book ID to delete:"
            read did
            grep -v "^$did|" $file > temp.txt
            mv temp.txt $file
            echo "Record Deleted!"
            ;;

        5)
            echo "Enter Book ID to update:"
            read uid
            grep "^$uid|" $file
            echo "Enter new Book Name:"
            read newname
            echo "Enter new Author:"
            read newauthor
            echo "Enter new Quantity:"
            read newqty
            sed -i "/^$uid|/c\\$uid|$newname|$newauthor|$newqty" $file
            echo "Record Updated!"
            ;;

        6)
            echo "Exiting..."
            break
            ;;

        *)
            echo "Invalid Choice!"
            ;;
    esac
done

