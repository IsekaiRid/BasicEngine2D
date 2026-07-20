#pragma once

#include <Basic2D/Engine.h>

// ============================================================
// RULER POSITION TOOL v5.0 — INTERACTIVE BOX
// ============================================================
// Cara pakai:
//   1. Tekan T     = spawn box di posisi mouse
//   2. Drag sudut  = resize box (4 titik sudut)
//   3. Drag rotasi = putar box (titik ke-5 di kanan tengah)
//   4. Tekan T     = lock/simpan posisi → LOG hasil
//   5. Tekan R     = toggle tool ON/OFF
//   6. Klik kanan  = cancel/hapus box yang sedang diedit
//
// Visual:
//   • Box hijau transparan = box yang sedang diedit
//   • 4 titik sudut warna  = bisa di-drag
//   • Titik rotasi ungu    = drag untuk putar
//   • Titik tengah putih   = center point
// ============================================================

struct RulerTool
{
    bool active = false;
    bool hasBox = false;        // apakah sedang ada box yang diedit
    bool isDragging = false;    // sedang drag sesuatu
    int dragPointIndex = -1;    // -1=none, 0-3=sudut, 4=rotasi

    glm::vec2 boxCenter = {0.0f, 0.0f};
    glm::vec2 boxSize = {100.0f, 100.0f};  // default size
    float boxRotation = 0.0f;

    glm::vec2 currentMouseWorld = {0.0f, 0.0f};
    glm::vec2 dragOffset = {0.0f, 0.0f};   // offset mouse saat mulai drag

    // Hasil akhir (setelah lock)
    glm::vec2 measuredPos = {0.0f, 0.0f};
    glm::vec2 measuredSize = {0.0f, 0.0f};
    float measuredRotation = 0.0f;
    bool hasMeasurement = false;
};

static RulerTool rulerTool;

// ============================================================
// KONVERSI SCREEN → WORLD SPACE
// ============================================================
inline glm::vec2 ScreenToWorld(glm::vec2 screenPos)
{
    glm::vec2 screenSize = renderWindows.ScreenSize;
    glm::vec2 baseSize = (gameCam.targetResolution.x > 0.0f && gameCam.targetResolution.y > 0.0f)
                         ? gameCam.targetResolution : screenSize;

    return {
        gameCam.pos.x + (screenPos.x / screenSize.x * baseSize.x) / gameCam.zoom.x,
        gameCam.pos.y + (screenPos.y / screenSize.y * baseSize.y) / gameCam.zoom.y
    };
}

// ============================================================
// HITUNG 4 SUDUT BOX DARI CENTER + SIZE + ROTATION
// ============================================================
inline void GetBoxCorners(glm::vec2 center, glm::vec2 size, float rotationDeg, glm::vec2 outCorners[4])
{
    float rad = glm::radians(rotationDeg);
    float c = cosf(rad);
    float s = sinf(rad);

    glm::vec2 half = size * 0.5f;

    // Sudut lokal — FLIP Y karena world space Y+ ke BAWAH
    // Screen/World: Y=0 di TOP, Y meningkat ke BAWAH
    // Jadi: "top" = Y lebih kecil, "bottom" = Y lebih besar
    glm::vec2 local[4] = {
        {-half.x,  half.y},  // top-left    (x-, y+ dalam screen coords)
        { half.x,  half.y},  // top-right   (x+, y+ dalam screen coords)
        { half.x, -half.y},  // bottom-right(x+, y- dalam screen coords)
        {-half.x, -half.y}   // bottom-left (x-, y- dalam screen coords)
    };

    // Apply rotasi + translate
    // Rotasi: positif = searah jarum jam (karena Y ke bawah)
    for (int i = 0; i < 4; i++)
    {
        outCorners[i].x = center.x + (local[i].x * c - local[i].y * s);
        outCorners[i].y = center.y + (local[i].x * s + local[i].y * c);
    }
}

// ============================================================
// HITUNG POSISI TITIK ROTASI (di kanan tengah box)
// ============================================================
inline glm::vec2 GetRotationHandlePos(glm::vec2 center, glm::vec2 size, float rotationDeg)
{
    float rad = glm::radians(rotationDeg);
    float c = cosf(rad);
    float s = sinf(rad);

    // Titik di kanan tengah (local: size.x/2 + offset, 0)
    // Y=0 di sini karena kanan tengah — tidak perlu flip Y
    float offset = 30.0f;
    glm::vec2 local = {size.x * 0.5f + offset, 0.0f};

    return {
        center.x + (local.x * c - local.y * s),
        center.y + (local.x * s + local.y * c)
    };
}

// ============================================================
// HITUNG JARAK DARI MOUSE KE TITIK
// ============================================================
inline float DistanceMouseToPoint(glm::vec2 mouseWorld, glm::vec2 point)
{
    return glm::length(mouseWorld - point);
}

// ============================================================
// TOGGLE RULER
// ============================================================
inline void Ruler_Toggle()
{
    rulerTool.active = !rulerTool.active;
    if (rulerTool.active)
    {
        LOG_TRACE("=== RULER TOOL AKTIF ===");
        LOG_TRACE("T = spawn box di mouse");
        LOG_TRACE("Drag sudut = resize");
        LOG_TRACE("Drag ungu = rotasi");
        LOG_TRACE("T lagi = lock/simpan");
        LOG_TRACE("Klik kanan = cancel");
        LOG_TRACE("R = matikan");
    }
    else
    {
        LOG_TRACE("=== RULER TOOL NONAKTIF ===");
        rulerTool.hasBox = false;
        rulerTool.isDragging = false;
        rulerTool.hasMeasurement = false;
    }
}

inline void Ruler_Enable()  { if (!rulerTool.active) Ruler_Toggle(); }
inline void Ruler_Disable() { if (rulerTool.active)  Ruler_Toggle(); }

// ============================================================
// SPAWN BOX BARU DI POSISI MOUSE
// ============================================================
inline void Ruler_SpawnBox()
{
    if (!rulerTool.hasBox)
    {
        // Spawn box baru di posisi mouse
        rulerTool.boxCenter = rulerTool.currentMouseWorld;
        rulerTool.boxSize = {100.0f, 100.0f};  // default
        rulerTool.boxRotation = 0.0f;
        rulerTool.hasBox = true;
        rulerTool.hasMeasurement = false;
        LOG_TRACE("[Ruler] Box spawn di %.1f, %.1f", rulerTool.boxCenter.x, rulerTool.boxCenter.y);
    }
    else
    {
        // Lock/simpan box yang sudah ada
        rulerTool.measuredPos = rulerTool.boxCenter;
        rulerTool.measuredSize = rulerTool.boxSize;
        rulerTool.measuredRotation = rulerTool.boxRotation;
        rulerTool.hasMeasurement = true;
        rulerTool.hasBox = false;  // hapus box edit, simpan hasil

        LOG_TRACE("");
        LOG_TRACE("========== RULER LOCKED ==========");
        LOG_TRACE("Posisi (Center): %.1f, %.1f", rulerTool.measuredPos.x, rulerTool.measuredPos.y);
        LOG_TRACE("Ukuran (Size):   %.1f, %.1f", rulerTool.measuredSize.x, rulerTool.measuredSize.y);
        LOG_TRACE("Rotasi (Deg):    %.1f°", rulerTool.measuredRotation);
        LOG_TRACE("");
        LOG_TRACE("COLLIDER CODE:");
        LOG_TRACE("RegisterWorldCollider({%.1ff, %.1ff}, {%.1ff, %.1ff}, square, ColliderCategory::Static, %.1ff);",
                  rulerTool.measuredPos.x, rulerTool.measuredPos.y,
                  rulerTool.measuredSize.x, rulerTool.measuredSize.y,
                  rulerTool.measuredRotation);
        LOG_TRACE("==================================");
    }
}

// ============================================================
// CANCEL BOX
// ============================================================
inline void Ruler_CancelBox()
{
    if (rulerTool.hasBox)
    {
        rulerTool.hasBox = false;
        rulerTool.isDragging = false;
        LOG_TRACE("[Ruler] Box dibatalkan");
    }
}

// ============================================================
// UPDATE RULER
// ============================================================
inline void Ruler_Update()
{
    if (!rulerTool.active) return;

    // Toggle dengan tombol R
    if (Pressed(ActionCode::R)) { Ruler_Toggle(); return; }

    // Update posisi mouse di world space
    rulerTool.currentMouseWorld = ScreenToWorld(MousePos());

    // Tekan T = spawn box atau lock
    if (Pressed(ActionCode::T))
    {
        Ruler_SpawnBox();
        return;
    }

    // Klik kanan = cancel box
    if (Pressed(ActionCode::MouseRight))
    {
        Ruler_CancelBox();
        return;
    }

    // Kalau tidak ada box, tidak perlu proses drag
    if (!rulerTool.hasBox) return;

    // Hitung posisi 4 sudut dan titik rotasi
    glm::vec2 corners[4];
    GetBoxCorners(rulerTool.boxCenter, rulerTool.boxSize, rulerTool.boxRotation, corners);
    glm::vec2 rotHandle = GetRotationHandlePos(rulerTool.boxCenter, rulerTool.boxSize, rulerTool.boxRotation);

    // --- MULAI DRAG (KLIK KIRI) ---
    if (Pressed(ActionCode::MouseLeft) && !rulerTool.isDragging)
    {
        float grabRadius = 15.0f;  // radius untuk "nangkep" titik
        float bestDist = grabRadius;
        int bestIdx = -1;

        // Cek 4 sudut
        for (int i = 0; i < 4; i++)
        {
            float d = DistanceMouseToPoint(rulerTool.currentMouseWorld, corners[i]);
            if (d < bestDist)
            {
                bestDist = d;
                bestIdx = i;
            }
        }

        // Cek titik rotasi
        float rotDist = DistanceMouseToPoint(rulerTool.currentMouseWorld, rotHandle);
        if (rotDist < bestDist)
        {
            bestDist = rotDist;
            bestIdx = 4;  // index 4 = rotasi
        }

        if (bestIdx >= 0)
        {
            rulerTool.isDragging = true;
            rulerTool.dragPointIndex = bestIdx;

            // Simpan offset supaya titik tidak "teleport" ke mouse
            if (bestIdx < 4)
                rulerTool.dragOffset = corners[bestIdx] - rulerTool.currentMouseWorld;
            else
                rulerTool.dragOffset = rotHandle - rulerTool.currentMouseWorld;
        }
    }

    // --- SEDANG DRAG ---
    if (rulerTool.isDragging && Held(ActionCode::MouseLeft))
    {
        glm::vec2 targetPos = rulerTool.currentMouseWorld + rulerTool.dragOffset;

        if (rulerTool.dragPointIndex >= 0 && rulerTool.dragPointIndex < 4)
        {
            // --- DRAG SUDUT = RESIZE ---
            int idx = rulerTool.dragPointIndex;

            // Sudut yang berlawanan (tidak bergerak)
            int oppositeIdx = (idx + 2) % 4;
            glm::vec2 opposite = corners[oppositeIdx];

            // Center baru = tengah antara sudut yang ditarik dan sudut berlawanan
            rulerTool.boxCenter = (targetPos + opposite) * 0.5f;

            // Size baru = jarak antara kedua sudut (dalam local space)
            glm::vec2 diff = targetPos - opposite;

            // Inverse rotation: rotate diff by -rotation
            float rad = glm::radians(-rulerTool.boxRotation);
            float c = cosf(rad);
            float s = sinf(rad);

            glm::vec2 localDiff;
            localDiff.x = diff.x * c - diff.y * s;
            localDiff.y = diff.x * s + diff.y * c;

            rulerTool.boxSize = glm::abs(localDiff);
        }
        else if (rulerTool.dragPointIndex == 4)
        {
            // --- DRAG TITIK ROTASI = ROTASI ---
            glm::vec2 toHandle = targetPos - rulerTool.boxCenter;
            rulerTool.boxRotation = glm::degrees(atan2f(toHandle.y, toHandle.x));
        }
    }

    // --- SELESAI DRAG (LEPAS KLIK) ---
    if (rulerTool.isDragging && Released(ActionCode::MouseLeft))
    {
        rulerTool.isDragging = false;
        rulerTool.dragPointIndex = -1;
    }
}

// ============================================================
// RENDER RULER VISUAL — DIRECT OPENGL
// ============================================================
inline void Ruler_RenderVisual()
{
    if (!rulerTool.active) return;

    // Simpan state OpenGL
    GLboolean blendEnabled = glIsEnabled(GL_BLEND);
    GLint lastProgram;  glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
    GLint lastVAO;      glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVAO);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(shaderData.collision.program.programId);
    glBindVertexArray(shaderData.collision.VAO);
    GetCameraViewProjection(gameCam, shaderData.collision.program.programId);

    // --- CROSSHAIR DI POSISI MOUSE ---
    {
        glm::vec2 m = rulerTool.currentMouseWorld;
        float cs = 10.0f;
        CollisionRenderData ch[2];
        ch[0] = {{m.x - cs, m.y}, {cs * 2.0f, 1.5f}, 0.0f, (int)square, {}, {0.0f, 1.0f, 0.0f, 0.7f}};
        ch[1] = {{m.x, m.y - cs}, {1.5f, cs * 2.0f}, 0.0f, (int)square, {}, {0.0f, 1.0f, 0.0f, 0.7f}};
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderData.collision.collisionDataSSBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(CollisionRenderData) * 2, ch);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shaderData.collision.collisionDataSSBO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 2);
    }

    // --- RENDER BOX YANG SEDANG DIEDIT ---
    if (rulerTool.hasBox)
    {
        glm::vec2 corners[4];
        GetBoxCorners(rulerTool.boxCenter, rulerTool.boxSize, rulerTool.boxRotation, corners);
        glm::vec2 rotHandle = GetRotationHandlePos(rulerTool.boxCenter, rulerTool.boxSize, rulerTool.boxRotation);

        // Gambar box utama (hijau transparan)
        CollisionRenderData box;
        box.Position = rulerTool.boxCenter;
        box.Size = rulerTool.boxSize;
        box.Rotation = rulerTool.boxRotation;
        box.Type = (int)square;
        box.Color = {0.0f, 1.0f, 0.0f, 0.35f};

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderData.collision.collisionDataSSBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(CollisionRenderData), &box);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shaderData.collision.collisionDataSSBO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);

        // Gambar 4 titik sudut
        CollisionRenderData pts[4];
        for (int i = 0; i < 4; i++)
        {
            pts[i].Position = corners[i];
            pts[i].Size = {10.0f, 10.0f};
            pts[i].Rotation = 0.0f;
            pts[i].Type = (int)circle;

            // Warna berbeda per sudut
            switch (i)
            {
                case 0: pts[i].Color = {1.0f, 0.0f, 0.0f, 0.9f}; break; // Merah (TL)
                case 1: pts[i].Color = {1.0f, 1.0f, 0.0f, 0.9f}; break; // Kuning (TR)
                case 2: pts[i].Color = {0.0f, 1.0f, 1.0f, 0.9f}; break; // Cyan (BR)
                case 3: pts[i].Color = {1.0f, 0.0f, 1.0f, 0.9f}; break; // Magenta (BL)
            }
        }
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(CollisionRenderData) * 4, pts);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 4);

        // Gambar titik rotasi (ungu)
        CollisionRenderData rot;
        rot.Position = rotHandle;
        rot.Size = {12.0f, 12.0f};
        rot.Rotation = 0.0f;
        rot.Type = (int)circle;
        rot.Color = {0.8f, 0.0f, 0.8f, 0.9f}; // Ungu

        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(CollisionRenderData), &rot);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);

        // Gambar garis dari center ke titik rotasi
        glm::vec2 diff = rotHandle - rulerTool.boxCenter;
        float len = glm::length(diff);
        if (len > 0.5f)
        {
            float angle = atan2f(diff.y, diff.x);
            CollisionRenderData line;
            line.Position = (rulerTool.boxCenter + rotHandle) * 0.5f;
            line.Size = {len, 1.5f};
            line.Rotation = glm::degrees(angle);
            line.Type = (int)square;
            line.Color = {0.8f, 0.0f, 0.8f, 0.5f};

            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(CollisionRenderData), &line);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
        }

        // Gambar titik tengah (putih)
        CollisionRenderData center;
        center.Position = rulerTool.boxCenter;
        center.Size = {8.0f, 8.0f};
        center.Rotation = 0.0f;
        center.Type = (int)circle;
        center.Color = {1.0f, 1.0f, 1.0f, 0.9f};

        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(CollisionRenderData), &center);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
    }

    // --- RENDER HASIL AKHIR (SETelah lock) ---
    if (rulerTool.hasMeasurement)
    {
        CollisionRenderData result[2];
        result[0] = {rulerTool.measuredPos, rulerTool.measuredSize, rulerTool.measuredRotation,
                     (int)square, {}, {0.0f, 1.0f, 0.0f, 0.5f}}; // Hijau
        result[1] = {rulerTool.measuredPos, {12.0f, 12.0f}, 0.0f,
                     (int)circle, {}, {1.0f, 1.0f, 1.0f, 0.9f}}; // Putih center

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderData.collision.collisionDataSSBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(CollisionRenderData) * 2, result);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shaderData.collision.collisionDataSSBO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 2);
    }

    // Restore state
    glUseProgram(lastProgram);
    glBindVertexArray(lastVAO);
    if (!blendEnabled) glDisable(GL_BLEND);
}