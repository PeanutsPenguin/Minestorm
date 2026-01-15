# Minestorm 
School project made in my first year of game programming at ISART Digital montreal. The objective was to reproduce the game "Minestorm" in C with a good memory management and the SAT algorithm to check collision. 
# Demo

https://github.com/user-attachments/assets/d7a288ce-1c9e-468c-8196-fd4b23e66f8e

# Code 
The main features of this game was the enemies and the collision with them. In order to do this we learned the SAT principle and manage to write it in C to check collision between object in the world.\
The idea of SAT is very simple. We check all point of a form, get the max and min of it, same with another form and compare the values.\
If there is a gap then no collisions, else, collision is returned. Here is how we did it : 

```cpp
/*Check the Max and Min of a dot product between a normal vector given and vertexs of two form given*/
bool            check_max_and_min(Vector2 normedVector, Vector2* vertexArrayOne, Vector2* vertexArrayTwo, unsigned int nbrOfVErtexOne, unsigned int nbrOfVertexTwo)
{
    //Initialize the maximum and minimum values of both form
    float   MAXONE = 0;
    float   MINONE = 0;
    float   MAXTWO = 0;
    float   MINTWO = 0;
    float   result = 0;

    for(unsigned int i = 0; i < nbrOfVErtexOne; i++)                                                             //Doing the dot product for every vertex of the first form
    {
        result = (normedVector.x * vertexArrayOne[i].x) + (normedVector.y * vertexArrayOne[i].y);  
        if (i == 0)                                                                                     //If it's the first vertex initialize the max and min values to the first value
        {
            MAXONE = result;
            MINONE = result;
        }               
        else if (result > MAXONE)                                                                            //If the result is superior to the max, then the max is the result 
        {       
            MAXONE = result;
        }
        else if (result < MINONE)                                                                            //If the result is inferior to the min, the the min s the result
        {
            MINONE = result;
        }
    }

    for (unsigned int i = 0; i < nbrOfVertexTwo; i++)                                                             //Doing the dot product for every vertex of the second form
    {
        result = (normedVector.x * vertexArrayTwo[i].x) + (normedVector.y * vertexArrayTwo[i].y); 
        if (i == 0)                                                                                     //If it's the first vertex initialize the max and min values to the first value
        {
            MAXTWO = result;
            MINTWO = result;
        }
        else if (result > MAXTWO)                                                                            //If the result is superior to the max, then the max is the result 
        {
            MAXTWO = result;
        }
        else if (result < MINTWO)                                                                            //If the result is inferior to the min, the the min s the result
        {   
            MINTWO = result;
        }
    }

    if (!(MAXONE >= MINTWO && MAXTWO >= MINONE))                                                        //If there is a gap between them, return false
    {
        return false;
    }
    else                                                                                                //Else it's true
        return true;    
}
```

This works perfectly if the objects are two perfectly convex form. On the other hand, if one of the form is convex, the algorithm might call a collision when there's not. That's because the form as a sort of "Hole" in them.\
To handle this we divide the convex form into small concaves one and compare them. Here's a snippet of the "division" code : 

```cpp
/*Use the SAT formula to check a collision between a convexe object and a concave one*/
bool            SAT_between_convexe_and_concave(GameObject* formOne, GameObject* formTwo)
{
    if (!formOne->active || !formTwo->active)                   
        return false;
        
    Vector2 normVecArray[3] = {(Vector2){0, 0}, (Vector2){0, 0}, (Vector2){0, 0}};
    bool result = false;

    for (int i = 0; i < formTwo->numberOfSides -1; i++)
    {
        Vector2 vertexMatrix[3] = {formTwo->vertexMatrix[i], formTwo->vertexMatrix[i+1], formTwo->center};                          //Initialize a Vector array with 2 vertex side by side and the center of the form so it's convexe triangle
        
        for (int j = 0; j < 2; j++)                                                                                                 //Calculate the normal vector array of this triangle
        {
            normVecArray[j] = vector_between_summits(vertexMatrix[j], vertexMatrix[j+1]);
            normVecArray[j] = vector_normal(normVecArray[j]);
        }

        normVecArray[2] = vector_between_summits(vertexMatrix[2], vertexMatrix[0]);
        normVecArray[2] = vector_normal(normVecArray[2]);

        for (int j = 0; j < formOne->numberOfSides; j++)                                                                        
        {
            result = check_max_and_min(formOne->normalVecArray[j], formOne->vertexMatrix,                                           //Check the max and min with the normal vector array of the first form
                                       vertexMatrix, formOne->numberOfSides, 3);
            if (result == false)                                                                                                    //If the result is false it means there is no collision so we can skip to the next triangle
                break;
        }

        if (result == false)                                                                                                        //If the result is already false it means that there is no collisions 
            continue;                                                                                                               //so no need to check witch the normal vector array of the tirangle
                

        for (int j = 0; j < 3; j++)
        {
            result = check_max_and_min(normVecArray[j], formOne->vertexMatrix,                                                      //Check the max and min with the normal vector array we just created
                                       vertexMatrix, formOne->numberOfSides, 3);
            if (result == false)                                                                                                    //If the result is false it means there's no collision so we skip to the next triangle 
                break;
        }

        if (result)                                                                                                                 //If the result it's true it means there's a collision so we're returning true
            return result;
    }

    Vector2 vertexMatrix[3] = {formTwo->vertexMatrix[formTwo->numberOfSides -1], formTwo->vertexMatrix[0], formTwo->center};        //Create an array with the last vertex, the first one, and the center of the concave form

    for (int j = 0; j < 2; j++)                                                                                                     //Calculate their normal vector 
    {
        normVecArray[j] = vector_between_summits(vertexMatrix[j], vertexMatrix[j+1]);
        normVecArray[j] = vector_normal(normVecArray[j]);
    }

    normVecArray[2] = vector_between_summits(vertexMatrix[2], vertexMatrix[0]);                                                     //Calculate the normal vector between the las vertex and the first one
    normVecArray[2] = vector_normal(normVecArray[2]);

    for (int j = 0; j < formOne->numberOfSides; j++)                                                                                //Check the max and min with the normal vector array of the first form
    {
        result = check_max_and_min(formOne->normalVecArray[j], formOne->vertexMatrix, vertexMatrix, formOne->numberOfSides, 3);
        if (!result)                                                                                                                //If the result is false t means there's no collision so we're returning false
            return result;
    }

    for (int j = 0; j < 3; j++)                                                                                                     //Check the max and min with the noraml vector array we just calculated
    {
        result = check_max_and_min(normVecArray[j], formOne->vertexMatrix, vertexMatrix, formOne->numberOfSides, 3);
        if (!result)                                                                                                                //If the result is false it mean there's no collision so we're returning false
            return result;
    }

    return result;                                                                                                                  //If the function reach the end it means it's true so we're returning true
}
```

That was the main thing of this project. Thank you for reading !

## Contact
<u>**Malo Sadoine**</u>

- [![Linkedin][LinkedIn]][LinkedIn-url]
- mal.sadoine@gmail.com

[LinkedIn]: https://img.shields.io/badge/linkedin-34a8eb?style=for-the-badge&logo=linkedin
[LinkedIn-url]: https://www.linkedin.com/in/malo-sadoine-098b7a254/

