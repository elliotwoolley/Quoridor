import React, {useEffect, useRef} from "react";

interface ConfettiParticle {
    x: number;
    y: number;
    velocityX: number;
    velocityY: number;
    angle: number;
    angularVelocity: number;
    size: number;
    color: string;
    opacity: number;
}

const randomInRange = (min: number, max: number): number => Math.random() * (max - min) + min;

interface ConfettiEffectProps {
    color: string;
}

const ConfettiEffect: React.FC<ConfettiEffectProps> = ({ color }) => {
    const canvasRef = useRef<HTMLCanvasElement | null>(null);
    const particles = useRef<ConfettiParticle[]>([]);
    const particlesSpawned = useRef<boolean>(false);
    const animationFrameId = useRef<number>(0);

    const createParticles = (count: number) => {
        const canvas = canvasRef.current;
        if (!canvas) {
            return;
        }

        if (particlesSpawned.current) {
            return;
        }
        particlesSpawned.current = true;

        for (let i = 0; i < count; i++) {
            particles.current.push({
                x: randomInRange(0, canvas.width),
                y: randomInRange(-300, 0),
                velocityX: randomInRange(-5, 5),
                velocityY: randomInRange(-3, 5),
                angle: randomInRange(0, 2 * Math.PI),
                angularVelocity: randomInRange(-0.1, 0.1),
                size: randomInRange(5, 10),
                color: color,
                opacity: 1,
            });
        }
    };

    const updateParticles = () => {
        const gravity = 0.01;
        particles.current.forEach((p, index) => {
            p.velocityY += gravity;
            p.x += p.velocityX;
            p.y += p.velocityY;
            p.angle += p.angularVelocity;

            if (p.x < 0 || p.x > (canvasRef.current?.width ?? 0) ||
                p.y > (canvasRef.current?.height ?? 0)
            ) {
                particles.current.splice(index, 1);
            }
        });
    };

    const drawParticles = (context: CanvasRenderingContext2D) => {
        particles.current.forEach((p) => {
            context.save();
            context.translate(p.x, p.y);
            context.rotate(p.angle);
            context.fillStyle = p.color;
            context.globalAlpha = p.opacity;
            context.fillRect(-p.size / 2, -p.size / 2, p.size, p.size);
            context.restore();
        });
    };

    const animate = () => {
        const canvas = canvasRef.current;
        if (!canvas) {
            return;
        }

        const context = canvas.getContext("2d");
        if (!context) {
            return;
        }

        context.clearRect(0, 0, canvas.width, canvas.height);
        updateParticles();
        drawParticles(context);

        if (particles.current.length > 0) {
            animationFrameId.current = requestAnimationFrame(animate);
        }
    };

    useEffect(() => {
        const canvas = canvasRef.current;
        if (!canvas) {
            return;
        }

        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;

        createParticles(1200);
        animate();

        const handleResize = () => {
            if (canvas) {
                canvas.width = window.innerWidth;
                canvas.height = window.innerHeight;
            }
        };
        window.addEventListener("resize", handleResize);

        return () => {
            window.removeEventListener("resize", handleResize);
            cancelAnimationFrame(animationFrameId.current);
        };
    });

    return (
        <canvas
            ref={canvasRef}
            style={{
                position: "fixed",
                backgroundColor: "transparent",
                top: 0,
                left: 0,
                pointerEvents: "none",
                zIndex: 9999,
            }}
        />
    );
};

export default ConfettiEffect;