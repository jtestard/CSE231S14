## Customization : Optimizing Compiler Technology for SELF, a dynamically typed object oriented programming language 

### Jules's comments
 - The author presents the implementation of a dynamically typed object oriented language called SELF. The SELF language performs twice better than other dynamically typed languages such as Smalltalk, because it uses novel optimizations other languages do not, such as : 
   - customized compilation, 
   - type prediction
   - call splitting
 - The system currently has limitations. For instance, it can only extract types from method receivers, but not from method arguments.  
 - The author suggest using type flow analysis to predict more types (such as method arguments) at compile time, and a more poweful type of message splitting.   