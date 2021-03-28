# Using C to Generate HTML from Markdown
Mar 2021

Recently, I took it upon myself to create a static site generator in order to improve my personal website, without needing to create HTML templates that need to be copied and pasted. I wanted a simple, fast and lightweight site generator that would take a bunch of pages in the form of markdown and generate HTML from them.

In the end, I came out with a generator that will render this site in less than a millisecond, and supports all the essential features of markdown. [Here is the complete code for it](https://raw.githubusercontent.com/georgelam6/georgelam6.github.io/master/generator.c).

## Why C?
Some might argue that C isn't really the best language to use for creating a markdown parser, and I would agree. Many "modern" languages have features such as build-in string manipulation that make a task such as this extremely easier. However, I like C, I find it a very nice language to use. And I wanted to brush up on my pointer arithmetic a bit, since string manipulation is quite heavy in that department.

## Scope
I didn't end up implementing every markdown feature, rather, I only implemented the ones that I anticipated needing for this site in the near future. This includes headers, paragraphs, links, lists and images, as well as bold and italic text.

## Algorithm overview
The way I chose to implement this algorithm is as follows:
 - Loop over the lines of markdown.
 - Check to see if the line starts with any hash-tags, indicating a header.
 - Check to see if the line starts with a "-" character, indicating a list.
 - Iterate the characters in the line, parsing links, images, bold and italics.
 - Output HTML accordingly.

## Parsing headers
This is quite simple. First, a check is carried out to see if the current line begins with a hashtag. Then, the number of hashtags after that are counted to check what header tag should be used, eg. Two hashes is a `&lt;h2>` tag, three is a `&lt;h3>`, etc.

## Parsing lists
This actually took me quite a while to figure out. Checking to see if the line begins with a "-" and inserting a `&lt;li>` tag is easy, it's a similar process to parsing headers. The problem arises with adding the corresponding `&lt;ul>` and `&lt;/ul>` tags in the correct places. Ideally we want them at the start and end of a list, not around each element, meaning the algorithm has to look back on itself to see if it should insert the ending `&lt;/ul>` tag.

I did this by storing the previous line as well as the current line while iterating the markdown file. This way, whenever a new line is encountered, we can check if it is a list item and if the previous line was a list item. If the previous line is a list item and the current one isn't, we know it's the end of a list and can insert the `&lt;/ul>` tag.

## Parsing links & images
Parsing links and images is fairly straighforward, once you know how to get the characters between two other characters. The markdown for a link is shown here:

`[amazing link](www.address.com)`

It needs to be converted into this HTML form:

`&lt;a href="www.address.com">amazing link&lt;/a>`

The way I did this was by iterating each paragraph one character at a time. Yes, this is bad for performance, but since the characters are grouped in memory, the program can make efficient use of cache, so the performance hit isn't too great. When a `[` is encountered, we start recording the text until the next `]` character, giving us the link's display text. After that, we look for the corresponding `(`, which should come exactly after the `]`. Then, we record everything until the next `)`, which gives us the string of where the link should link to. Then, we can easily generate an HTML string via. `sprintf`.

The algorithm for images is almost identical. The only exception is that there is a `!` before the first `[`, and the image's alt-text is between the square brackers, and the the image's source is between the round brackets.

## Things to consider
This markdown parser is no where near complete. It doesn't have support for many widely used features of markdown, here are a couple of common ones that you may need to implement for your markdown renderer:
 - Tables
 - Nested lists
 - Code snippets
 - Block quotes
 - Horizontal rules

You will find that most of these are fairly trivial to implement. Others, however, such as tables and nested lists, are much more difficult.
