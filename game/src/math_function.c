#include "game_manager.h"

/*Function to calaculate a Vector between two summits given*/
Vector2         vector_between_summits(Vector2 firstSummit, Vector2 secondSummit)
{
    Vector2     result = {0};                       //Initialize the vector result to 0

    result.x = secondSummit.x - firstSummit.x;      //Calculate the .x of the result vector
    result.y = secondSummit.y - firstSummit.y;      //Calculate the .u of the result vector

    return result;                                  //Return the Vector result
}

/*Function to calaculate the norm of a given vector*/
float           vector_norm(Vector2 toCvonvert)
{
    float   resultx = toCvonvert.x * toCvonvert.x;      //Calculate the square of the .x
    float   resulty = toCvonvert.y * toCvonvert.y;      //Calculate the squqre of the .y

    float   result = sqrt(resultx+resulty);             //Doing the square root of the result

    return result;                                      //Return the result
}

/*Function to calculate the noraml vector of a given vector*/
Vector2         vector_normal(Vector2 toNormalize)
{
    Vector2 result = {0};               //Initialize the result vector to 0

    result.x = toNormalize.y * -1;      //Invert the x and the y
    result.y = toNormalize.x;

    return result;                      //Return the result
}

/*Update all the vertex position of a given GameObject*/
void            update_vertex_array(GameObject* object)
{
    for (int i = 0; i < object->numberOfSides; i++)                                                         //Doing this for every vertex                                                                                  
    {
        object->vertexMatrix[i].x =  object->radiusArray[i]*                                                //Multiply by the radius so it has the right size   
                                        cos(TWOPI * (object->rotation + (object->angleArray[i])) / 360)     //Formula to get the right x position for the vertex
                                        + object->center.x;                                                 //Place it next to the center
        
        object->vertexMatrix[i].y =  object->radiusArray[i]*                                                //Multiply by the radius so it has the right size          
                                        sin(TWOPI * (object->rotation + (object->angleArray[i])) / 360)     //Formula to get the right y position for the vertex
                                        + object->center.y;                                                 //Place it next to the center
    }
}

/*Function to move a given vector to another given vector*/
Vector2         move_to_vector(Vector2 vecToMove, Vector2 direction, float speed)
{
    Vector2 result = {0};

    float   newX = direction.x - vecToMove.x;                               //Calculate the distance between the x position of the center and x position of our direction vertex
    float   newY = direction.y - vecToMove.y;                               //Same but with y position
    float   norme = (newX*newX) + (newY*newY);                              //Get the norme of this vectore
    
    if (norme <= 1)                                                         //If it equals to 1 it means they're near eachother so we returning the direction vector
        return direction;

    float   distance = sqrt(norme);                                         //If it's not equals to zero we get the distance between them

    result.x = vecToMove.x + newX/distance*speed;                           //Moving the x position to the x of the direction vertex but divide by the distance so he is doing step by step
    result.y = vecToMove.y + newY/distance*speed;                           //Same but with y

    return result;                                                          //Return the new Vector2
}

/*Update all the normal vector of a given GameObject*/
void            update_vector_normal(GameObject* object)
{
    if (!object->active)                                                                    //If the object is inactive, get out of the function
        return;

    for (unsigned int i = 0; i < object->numberOfSides - 1; i++)                            //Doing this for every side of the object exept the last one
    {
        object->normalVecArray[i] = vector_between_summits(object->vertexMatrix[i],         //Calculate the vector beween a summit and the next one
                                                           object->vertexMatrix[i+1]);
        object->normalVecArray[i] = vector_normal(object->normalVecArray[i]);               //Calculate the normal Vector of the vector calculate just before
    }

    //Doing the same thing but with the last and the first vertex
    object->normalVecArray[object->numberOfSides - 1] = 
    vector_between_summits(object->vertexMatrix[object->numberOfSides - 1], 
                           object->vertexMatrix[0]);

    object->normalVecArray[object->numberOfSides - 1] = 
    vector_normal(object->normalVecArray[object->numberOfSides - 1]);
}

/*Use the SAT formula to check a collision between to convexe object*/
bool            SAT_between_form(GameObject* firstObject, GameObject* secondObject)
{
    if (!firstObject->active || !secondObject->active)                                              //If one of the object is inactive, get out of the function
        return false;

    bool result = true;                                                                             //Initialize the result to true 

    for (int i = 0; i < firstObject->numberOfSides; i++)                                            //Check the Max and Min with the normal vector of the first form
    {
        result = check_max_and_min(firstObject->normalVecArray[i], 
                                   firstObject->vertexMatrix, secondObject->vertexMatrix, 
                                   firstObject->numberOfSides, secondObject->numberOfSides);
        if (result == false)                                                                        //If one is false it means that there's no collision so we return false
        {
            return result;
        }
    }

    for (int i = 0; i < secondObject->numberOfSides; i++)                                           //Check the Max and Min with the normal vector of the second form
    {
        result = check_max_and_min(secondObject->normalVecArray[i], 
                                   firstObject->vertexMatrix, secondObject->vertexMatrix, 
                                   firstObject->numberOfSides, secondObject->numberOfSides);
        if (result == false)                                                                        //If one is false it means that there's no collision so we return false
        {
            return result;
        }
    }

    return result;                                                                                  //If the function reach the end it means the result is true so we return true
}

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

/*Function to prevent a given GameObject to get out of the screen*/
void            border_collision (GameObject* entity)
{
    if (entity->center.x > 765)
    {
        entity->center.x = BACKGROUND_POS;
        // entity->rotation += 90;
    }
    else if (entity->center.x < BACKGROUND_POS)
    {
        entity->center.x = 765;
        // entity->rotation += 90;
    }

    if (entity->center.y > RECT_HEIGHT - 40)
    {
        entity->center.y = BACKGROUND_POS;
        // entity->rotation += 90;
    }
    else if (entity->center.y < BACKGROUND_POS)
    {
        entity->center.y = RECT_HEIGHT - 40;
        // entity->rotation += 90;
    }
}

/*Function to keep the rotation of a given GameObject between -360 and 360*/
void            stable_rotation(GameObject* entity)
{
    if (entity->rotation > 360)
    {
        entity->rotation = (int)entity->rotation%360;       //If the rotation is superior the 360, divide it by 360
    }
    if (entity->rotation < -360)
    {
        entity->rotation *= -1;                             //If the roation is inferior to -360, reset itto a positive value and divide it by 360
        entity->rotation = (int)entity->rotation % 360;
    }
}