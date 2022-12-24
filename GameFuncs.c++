// Calculates the drag constant for a given altitude
float GameFuncs::GetDragConstant(float LocalPosY)
{
    // Maximum altitude for which the drag constant is calculated
    const auto max_alt = 18300.0f;

    // Multiplier for the calculated drag constant
    const auto alt_mult = 1.225f;

    // Clamp the altitude to the maximum value
    const auto clamped_alt = fmin(LocalPosY, max_alt);

    // Unknown constants used in the calculation of the drag constant
    const auto unk_1 = 2.2871901e-19f, unk_2 = 5.8355603e-14f,
        unk_3 = 0.00000000353118f, unk_4 = 0.000095938703f;

    // Calculate the drag constant using the clamped altitude and the unknown constants
    return alt_mult *
        ((max_alt / std::fmax(LocalPosY, max_alt)) *
            ((((((unk_1 * clamped_alt) - unk_2) * clamped_alt) + unk_3) *
                clamped_alt) -
                unk_4) *
            clamped_alt +
            1.0f);
}

// Calculates the ballistic coefficient for a bullet based on its length, mass, caliber, and drag constant
float GameFuncs::GetBallisticCoeff(float BulletLength, float BulletMass, float BulletCaliber, float DragConstant)
{
    // Calculate the ballistic coefficient based on the given bullet properties and drag constant
    return -1.0f *
        (DragConstant * static_cast<float>(M_PI) * 0.5f *
            std::pow(BulletCaliber * 0.5f, 2.0f) *
            BulletLength) /
        BulletMass;
}

// Applies drag to the given bullet's velocity and updates its position based on the given ballistic coefficient
void GameFuncs::ApplyDrag(float BallisticCoeff, vec2& BulletVel, vec2& BulletPos)
{
    // Calculate the change in speed due to drag
    float delta_speed = (BallisticCoeff * this->time_step) * BulletVel.Length();

    // Calculate a velocity multiplier based on the change in speed
    float velocityMult = 1.0f;
    if (delta_speed < 1.0f || delta_speed > 1.0f) {
        velocityMult = (delta_speed / (1.0f - delta_speed)) + 1.0f;
    }

    // Apply the velocity multiplier and gravity to the bullet's velocity
    vec2 dragged_vel{
        BulletVel.x * velocityMult,
        (this->gravity * this->time_step) + (velocityMult * BulletVel.y)
        //BulletVel.z * velocityMult                                            //Z for when you transition to the game's 3D coords
    };

    // Update the bullet's velocity
    BulletVel = dragged_vel;


vec2 new_pos{
        (dragged_vel.x * this->time_step) + BulletPos.x,
        (dragged_vel.y * this->time_step) + BulletPos.y
        //(dragged.z * this->time_step) + BulletPos.z                           //Z for when you transition to the game's 3D coords
    };

    // Update the bullet's position
    BulletPos = new_pos;
}
